//
// Created by Stefan on 4/8/2025.
//

#include "engine/backend/vulkan.hpp"

#include <vulkan/vulkan.h>

namespace PT {

VulkanBackend::VulkanBackend() = default;

VulkanBackend::~VulkanBackend() = default;

void VulkanBackend::UpdateRenderPayload(const RenderPayload& payload) {
  m_Camera = &payload.Camera;
  m_ScenePrimitive = &payload.Scene;
}

void VulkanBackend::RenderToBuffer(const RenderCaptureSpecification& spec) {
  VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

  // 1. Create an image
  VkImageCreateInfo imageInfo{
      .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
      .imageType = VK_IMAGE_TYPE_2D,
      .format = format,
      .extent = {spec.Width, spec.Height, 1},
      .mipLevels = 1,
      .arrayLayers = 1,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .tiling = VK_IMAGE_TILING_LINEAR,
      .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
  };

  VkImage image;
  VK_CHECK(vkCreateImage(m_Context.Device(), &imageInfo, nullptr, &image));

  // 2. Allocate memory
  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(m_Context.Device(), image, &memRequirements);

  VkMemoryAllocateInfo allocInfo{
      .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      .allocationSize = memRequirements.size,
      .memoryTypeIndex = m_Context.FindMemoryType(
          memRequirements.memoryTypeBits,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
          )
  };

  VkDeviceMemory imageMemory;
  VK_CHECK(vkAllocateMemory(m_Context.Device(), &allocInfo, nullptr, &imageMemory));

  VK_CHECK(vkBindImageMemory(m_Context.Device(), image, imageMemory, 0));

  // 3. Command buffer
  VkCommandBufferAllocateInfo allocCommandInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = m_Context.CommandPool(),
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1
  };

  VkCommandBuffer cmd;
  VK_CHECK(vkAllocateCommandBuffers(m_Context.Device(), &allocCommandInfo, &cmd));

  VkCommandBufferBeginInfo beginInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
  };
  VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

  // 4. Transition image to transfer dst
  VkImageMemoryBarrier barrier{
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
      .srcAccessMask = 0,
      .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
      .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = image,
      .subresourceRange = {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1
      }
  };

  vkCmdPipelineBarrier(
      cmd,
      VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
      0,
      0, nullptr,
      0, nullptr,
      1, &barrier
  );

  // 5. Clear the image
  VkClearColorValue clearColor = {0.0f, 0.0f, 1.0f, 1.0f}; // blue

  VkImageSubresourceRange clearRange{
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
  };

  vkCmdClearColorImage(
      cmd,
      image,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      &clearColor,
      1,
      &clearRange
  );

  // 6. Transition to GENERAL so we can map it
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;

  vkCmdPipelineBarrier(
      cmd,
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT,
      0,
      0, nullptr,
      0, nullptr,
      1, &barrier
  );

  VK_CHECK(vkEndCommandBuffer(cmd));

  // 7. Submit and wait
  VkSubmitInfo submitInfo{
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &cmd
  };

  VK_CHECK(vkQueueSubmit(m_Context.GraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
  VK_CHECK(vkQueueWaitIdle(m_Context.GraphicsQueue()));

  // 8. Map and copy
  void* data;
  VK_CHECK(vkMapMemory(m_Context.Device(), imageMemory, 0, VK_WHOLE_SIZE, 0, &data));

  size_t rowPitch = spec.Width * 4;
  for (uint32_t y = 0; y < spec.Height; y++) {
    std::memcpy(
        spec.Buffer + y * spec.Width,
        static_cast<char*>(data) + y * rowPitch,
        rowPitch
    );
  }

  vkUnmapMemory(m_Context.Device(), imageMemory);

  // 9. Cleanup
  vkFreeMemory(m_Context.Device(), imageMemory, nullptr);
  vkDestroyImage(m_Context.Device(), image, nullptr);
}

}  // namespace PT
