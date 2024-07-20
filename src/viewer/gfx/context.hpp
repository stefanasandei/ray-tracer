//
// Created by Asandei Stefan on 20.07.2024.
//

#pragma once

#include "core/base.hpp"
#include "gfx/external.hpp"

namespace Viewer {

// ---- Helper Structs ----

struct Frame {
  nri::CommandAllocator *CommandAllocator;
  nri::CommandBuffer *CommandBuffer;
};

struct BackBuffer {
  nri::Descriptor *colorAttachment;
  nri::Texture *texture;
};

struct GfxContextSpecification {
  void *GFLWHandle;
  int32_t Width, Height;
};

// ---- The Graphics Context ----

class GfxContext {
 public:
  GfxContext(const GfxContextSpecification &spec);
  ~GfxContext();

  void RenderFrame(uint32_t frameIndex) noexcept;

 private:
  void InitDebugAllocator() noexcept;
  void InitDevice() noexcept;
  void InitNRI() noexcept;
  void InitStreamer() noexcept;
  void InitCommandQueue() noexcept;
  void InitFences() noexcept;
  void InitSwapchain(const GfxContextSpecification &spec) noexcept;
  void InitBuffers() noexcept;

 private:
  NRIInterface NRI = {};

  nri::Device *m_Device = nullptr;
  nri::Streamer *m_Streamer = nullptr;
  nri::SwapChain *m_SwapChain = nullptr;
  nri::CommandQueue *m_CommandQueue = nullptr;
  nri::Fence *m_FrameFence = nullptr;

  nri::MemoryAllocatorInterface m_MemoryAllocatorInterface = {};

  static constexpr uint32_t BUFFERED_FRAME_MAX_NUM = 2;
  static constexpr nri::SPIRVBindingOffsets SPIRV_BINDING_OFFSETS = {100, 200,
                                                                     300, 400};
  static constexpr uint32_t SWAP_CHAIN_TEXTURE_NUM = 2;

  std::array<Frame, BUFFERED_FRAME_MAX_NUM> m_Frames{};
  std::vector<BackBuffer> m_SwapChainBuffers;
};

// ---- Forward declare helper functions ----
void CreateDebugAllocator(
    nri::MemoryAllocatorInterface &memoryAllocatorInterface);
void DestroyDebugAllocator(
    nri::MemoryAllocatorInterface &memoryAllocatorInterface);

}  // namespace Viewer
