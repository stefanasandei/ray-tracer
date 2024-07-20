//
// Created by Asandei Stefan on 20.07.2024.
//

#include "gfx/context.hpp"

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#ifdef __APPLE__
#include "MetalUtility.h"
#endif

namespace Viewer {

GfxContext::GfxContext(const GfxContextSpecification &spec) {
  InitDebugAllocator();
  InitDevice();
  InitNRI();
  InitStreamer();
  InitCommandQueue();
  InitFences();
  InitSwapchain(spec);
  InitBuffers();
}

GfxContext::~GfxContext() {
  NRI.WaitForIdle(*m_CommandQueue);

  for (Frame &frame : m_Frames) {
    NRI.DestroyCommandBuffer(*frame.CommandBuffer);
    NRI.DestroyCommandAllocator(*frame.CommandAllocator);
  }

  for (BackBuffer &backBuffer : m_SwapChainBuffers)
    NRI.DestroyDescriptor(*backBuffer.colorAttachment);

  NRI.DestroyFence(*m_FrameFence);
  NRI.DestroySwapChain(*m_SwapChain);
  NRI.DestroyStreamer(*m_Streamer);

  nri::nriDestroyDevice(*m_Device);

  DestroyDebugAllocator(m_MemoryAllocatorInterface);
}

void GfxContext::InitDebugAllocator() noexcept {
  CreateDebugAllocator(m_MemoryAllocatorInterface);
}

void GfxContext::InitDevice() noexcept {
  nri::AdapterDesc bestAdapterDesc = {};
  uint32_t adapterDescsNum = 1;
  NRI_ABORT_ON_FAILURE(
      nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescsNum));

  printf("Using GPU %s\n", bestAdapterDesc.description);

  nri::DeviceCreationDesc deviceCreationDesc = {};
  deviceCreationDesc.graphicsAPI = nri::GraphicsAPI::VULKAN;
  deviceCreationDesc.enableAPIValidation = false;
  deviceCreationDesc.enableNRIValidation = true;
  deviceCreationDesc.spirvBindingOffsets = SPIRV_BINDING_OFFSETS;
  deviceCreationDesc.adapterDesc = &bestAdapterDesc;
  deviceCreationDesc.memoryAllocatorInterface = m_MemoryAllocatorInterface;
  NRI_ABORT_ON_FAILURE(nri::nriCreateDevice(deviceCreationDesc, m_Device));
}

void GfxContext::InitNRI() noexcept {
  NRI_ABORT_ON_FAILURE(nri::nriGetInterface(*m_Device,
                                            NRI_INTERFACE(nri::CoreInterface),
                                            (nri::CoreInterface *)&NRI));
  NRI_ABORT_ON_FAILURE(nri::nriGetInterface(*m_Device,
                                            NRI_INTERFACE(nri::HelperInterface),
                                            (nri::HelperInterface *)&NRI));
  NRI_ABORT_ON_FAILURE(
      nri::nriGetInterface(*m_Device, NRI_INTERFACE(nri::StreamerInterface),
                           (nri::StreamerInterface *)&NRI));
  NRI_ABORT_ON_FAILURE(
      nri::nriGetInterface(*m_Device, NRI_INTERFACE(nri::SwapChainInterface),
                           (nri::SwapChainInterface *)&NRI));
}

void GfxContext::InitStreamer() noexcept {
  nri::StreamerDesc streamerDesc = {};
  streamerDesc.dynamicBufferMemoryLocation = nri::MemoryLocation::HOST_UPLOAD;
  streamerDesc.dynamicBufferUsageBits =
      nri::BufferUsageBits::VERTEX_BUFFER | nri::BufferUsageBits::INDEX_BUFFER;
  streamerDesc.constantBufferMemoryLocation = nri::MemoryLocation::HOST_UPLOAD;
  streamerDesc.frameInFlightNum = BUFFERED_FRAME_MAX_NUM;
  NRI_ABORT_ON_FAILURE(NRI.CreateStreamer(*m_Device, streamerDesc, m_Streamer));
}

void GfxContext::InitCommandQueue() noexcept {
  NRI_ABORT_ON_FAILURE(NRI.GetCommandQueue(
      *m_Device, nri::CommandQueueType::GRAPHICS, m_CommandQueue));
}

void GfxContext::InitFences() noexcept {
  NRI_ABORT_ON_FAILURE(NRI.CreateFence(*m_Device, 0, m_FrameFence));
}

void GfxContext::InitSwapchain(const GfxContextSpecification &spec) noexcept {
  nri::Window NRIWindow = {};

#if _WIN32
  NRIWindow.windows.hwnd =
      glfwGetWin32Window(static_cast<GLFWwindow *>(spec.GFLWHandle));
#elif __linux__
  NRIWindow.x11.dpy = glfwGetX11Display();
  NRIWindow.x11.window =
      glfwGetX11Window(static_cast<GLFWwindow *>(spec.GFLWHandle));
#elif __APPLE__
  NRIWindow.metal.caMetalLayer =
      GetMetalLayer(static_cast<GLFWwindow *>(spec.GFLWHandle));
#endif

  nri::Format swapChainFormat;
  {
    nri::SwapChainDesc swapChainDesc = {};
    swapChainDesc.window = NRIWindow;
    swapChainDesc.commandQueue = m_CommandQueue;
    swapChainDesc.format = nri::SwapChainFormat::BT709_G22_8BIT;
    swapChainDesc.verticalSyncInterval = 0;
    swapChainDesc.width = (uint16_t)spec.Width;
    swapChainDesc.height = (uint16_t)spec.Height;
    swapChainDesc.textureNum = SWAP_CHAIN_TEXTURE_NUM;
    NRI_ABORT_ON_FAILURE(
        NRI.CreateSwapChain(*m_Device, swapChainDesc, m_SwapChain));

    uint32_t swapChainTextureNum;
    nri::Texture *const *swapChainTextures =
        NRI.GetSwapChainTextures(*m_SwapChain, swapChainTextureNum);
    swapChainFormat = NRI.GetTextureDesc(*swapChainTextures[0]).format;

    for (uint32_t i = 0; i < swapChainTextureNum; i++) {
      nri::Texture2DViewDesc textureViewDesc = {
          swapChainTextures[i], nri::Texture2DViewType::COLOR_ATTACHMENT,
          swapChainFormat};

      nri::Descriptor *colorAttachment;
      NRI_ABORT_ON_FAILURE(
          NRI.CreateTexture2DView(textureViewDesc, colorAttachment));

      const BackBuffer backBuffer = {colorAttachment, swapChainTextures[i]};
      m_SwapChainBuffers.push_back(backBuffer);
    }
  }
}

void GfxContext::InitBuffers() noexcept {
  for (Frame &frame : m_Frames) {
    NRI_ABORT_ON_FAILURE(
        NRI.CreateCommandAllocator(*m_CommandQueue, frame.CommandAllocator));
    NRI_ABORT_ON_FAILURE(
        NRI.CreateCommandBuffer(*frame.CommandAllocator, frame.CommandBuffer));
  }
}

void GfxContext::RenderFrame(uint32_t frameIndex) noexcept {
  const uint32_t bufferedFrameIndex = frameIndex % BUFFERED_FRAME_MAX_NUM;
  const Frame &frame = m_Frames[bufferedFrameIndex];

  if (frameIndex >= BUFFERED_FRAME_MAX_NUM) {
    NRI.Wait(*m_FrameFence, 1 + frameIndex - BUFFERED_FRAME_MAX_NUM);
    NRI.ResetCommandAllocator(*frame.CommandAllocator);
  }

  const uint32_t currentTextureIndex =
      NRI.AcquireNextSwapChainTexture(*m_SwapChain);
  BackBuffer &currentBackBuffer = m_SwapChainBuffers[currentTextureIndex];

  // Record
  nri::CommandBuffer *commandBuffer = frame.CommandBuffer;
  NRI.BeginCommandBuffer(*commandBuffer, nullptr);
  {
    nri::AttachmentsDesc attachmentsDesc = {};
    attachmentsDesc.colorNum = 1;
    attachmentsDesc.colors = &currentBackBuffer.colorAttachment;

    NRI.CmdBeginRendering(*commandBuffer, attachmentsDesc);
    {
      {
        nri::ClearDesc clearDesc = {};
        clearDesc.attachmentContentType = nri::AttachmentContentType::COLOR;
        clearDesc.value.color32f = {0.1F, 0.8F, 0.1F, 1.0F};

        NRI.CmdClearAttachments(*commandBuffer, &clearDesc, 1, nullptr, 0);
      }

      {
        //        RenderUI(NRI, NRI, *m_Streamer, *commandBuffer, 1.0f, true);
      }
    }
    NRI.CmdEndRendering(*commandBuffer);
  }
  NRI.EndCommandBuffer(*commandBuffer);

  {  // Submit
    nri::QueueSubmitDesc queueSubmitDesc = {};
    queueSubmitDesc.commandBuffers = &frame.CommandBuffer;
    queueSubmitDesc.commandBufferNum = 1;

    NRI.QueueSubmit(*m_CommandQueue, queueSubmitDesc);
  }

  // Present
  NRI.QueuePresent(*m_SwapChain);

  {  // Signaling after "Present" improves D3D11 performance a bit
    nri::FenceSubmitDesc signalFence = {};
    signalFence.fence = m_FrameFence;
    signalFence.value = 1 + frameIndex;

    nri::QueueSubmitDesc queueSubmitDesc = {};
    queueSubmitDesc.signalFences = &signalFence;
    queueSubmitDesc.signalFenceNum = 1;

    NRI.QueueSubmit(*m_CommandQueue, queueSubmitDesc);
  }
}

}  // namespace Viewer
