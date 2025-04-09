//
// Created by Stefan on 4/8/2025.
//

#include "engine/backend/vulkan.hpp"

#include <optick.h>
#include <vulkan/vulkan.h>

namespace PT {

VulkanBackend::VulkanBackend() = default;

VulkanBackend::~VulkanBackend() {
  vkFreeMemory(m_Context.Device(), m_ImageMemory, nullptr);
  vkDestroyImage(m_Context.Device(), m_Image, nullptr);
}

void VulkanBackend::UpdateRenderPayload(const RenderPayload& payload) {
  m_Camera = &payload.Camera;
  m_ScenePrimitive = &payload.Scene;
}

void VulkanBackend::InitPerFrameResources(uint32_t width, uint32_t height) {
  VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

  // 1. Create an image
  VkImageCreateInfo imageInfo = {.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                                 .imageType = VK_IMAGE_TYPE_2D,
                                 .format = format,
                                 .extent = {width, height, 1},
                                 .mipLevels = 1,
                                 .arrayLayers = 1,
                                 .samples = VK_SAMPLE_COUNT_1_BIT,
                                 .tiling = VK_IMAGE_TILING_LINEAR,
                                 .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                          VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                                 .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED};

  VK_CHECK(vkCreateImage(m_Context.Device(), &imageInfo, nullptr, &m_Image));

  // 2. Allocate memory
  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(m_Context.Device(), m_Image, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = memRequirements.size,
      .memoryTypeIndex =
          m_Context.FindMemoryType(memRequirements.memoryTypeBits,
                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)};

  VK_CHECK(vkAllocateMemory(m_Context.Device(), &allocInfo, nullptr,
                            &m_ImageMemory));

  VK_CHECK(vkBindImageMemory(m_Context.Device(), m_Image, m_ImageMemory, 0));

  // 3. Command buffer
  VkCommandBufferAllocateInfo allocCommandInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = m_Context.CommandPool(),
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1};

  VK_CHECK(
      vkAllocateCommandBuffers(m_Context.Device(), &allocCommandInfo, &m_Cmd));
}

float a = 0.0f, d = 0.01f;
void VulkanBackend::RenderToBuffer(const RenderCaptureSpecification& spec) {
  OPTICK_EVENT("Vulkan RenderToBuffer");

  // 1. create resources
  if ((m_ImageMemory == VK_NULL_HANDLE) ||
      (m_CurrentWidth != spec.Width || m_CurrentHeight != spec.Height)) {
    if (m_ImageMemory != VK_NULL_HANDLE) {
      vkFreeMemory(m_Context.Device(), m_ImageMemory, nullptr);
      vkDestroyImage(m_Context.Device(), m_Image, nullptr);
    }

    InitPerFrameResources(spec.Width, spec.Height);
  }

  VkCommandBufferBeginInfo beginInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};
  VK_CHECK(vkBeginCommandBuffer(m_Cmd, &beginInfo));

  // 2. Transition image to transfer dst
  VulkanContext::TransitionImage(m_Cmd, m_Image, VK_IMAGE_LAYOUT_UNDEFINED,
                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

  // 3. Clear the image
  a += d;  // just to debug if realtime works
  if (a < 0.0f || a > 1.0f) d = -d;
  VkClearColorValue clearColor = {0.0f, 0.0f, a, 1.0f};  // blue

  VkImageSubresourceRange clearRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                        .baseMipLevel = 0,
                                        .levelCount = 1,
                                        .baseArrayLayer = 0,
                                        .layerCount = 1};

  vkCmdClearColorImage(m_Cmd, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       &clearColor, 1, &clearRange);

  // 4. Transition to GENERAL so we can map it
  VulkanContext::TransitionImage(m_Cmd, m_Image,
                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                 VK_IMAGE_LAYOUT_GENERAL);

  VK_CHECK(vkEndCommandBuffer(m_Cmd));

  // 5. Submit and wait
  VkSubmitInfo submitInfo = {.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                             .commandBufferCount = 1,
                             .pCommandBuffers = &m_Cmd};

  VK_CHECK(
      vkQueueSubmit(m_Context.GraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
  VK_CHECK(vkQueueWaitIdle(m_Context.GraphicsQueue()));

  // 6. Map and copy
  void* data;
  VK_CHECK(vkMapMemory(m_Context.Device(), m_ImageMemory, 0, VK_WHOLE_SIZE, 0,
                       &data));

  size_t rowPitch = spec.Width * 4;
  for (uint32_t y = 0; y < spec.Height; y++) {
    std::memcpy(spec.Buffer + y * spec.Width,
                static_cast<char*>(data) + y * rowPitch, rowPitch);
  }

  vkUnmapMemory(m_Context.Device(), m_ImageMemory);
}

}  // namespace PT
