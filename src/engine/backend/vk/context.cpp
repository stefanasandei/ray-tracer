//
// Created by Stefan on 4/8/2025.
//

#include "engine/backend/vk/context.hpp"

namespace PT {

VulkanContext::VulkanContext() {
  SetupInstance();
  SetupDevice();
  SetupCommandBuffers();
}

void VulkanContext::SetupInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Ray Tracer";
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
  appInfo.apiVersion = VK_API_VERSION_1_4;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_Instance));

  m_DeletionQueue.Push([&] { vkDestroyInstance(m_Instance, nullptr); });
}

void VulkanContext::SetupDevice() {
  // 1. setup physical device
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
  ASSERT(deviceCount > 0);

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

  // find a proper physical device
  for (const auto& physicalDevice : devices) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    if (deviceProperties.apiVersion < VK_API_VERSION_1_3) continue;

    // find a queue family that supports graphics
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                             nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(
        queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                             queueFamilyProperties.data());

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        m_GraphicsQueueIndex = i;
        break;
      }
    }

    if (m_GraphicsQueueIndex >= 0) {
      // found the one!
      m_PhysicalDevice = physicalDevice;
      break;
    }
  }

  ASSERT(m_GraphicsQueueIndex >= 0);

  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);
  printf("Selected GPU: %s\n", deviceProperties.deviceName);

  // 2. setup logical device

  // start out by enabling cool extensions
  VkPhysicalDeviceExtendedDynamicStateFeaturesEXT
      enableExtendedDynamicStateFeatures = {
          .sType =
              VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
          .extendedDynamicState = VK_TRUE};

  VkPhysicalDeviceVulkan13Features enableVulkan13Features = {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
      .pNext = &enableExtendedDynamicStateFeatures,
      .synchronization2 = VK_TRUE,
      .dynamicRendering = VK_TRUE,
  };

  VkPhysicalDeviceVulkan14Features enableVulkan14Features = {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
      .pNext = &enableVulkan13Features,
      .hostImageCopy = true
  };

  VkPhysicalDeviceFeatures2 enableDeviceFeatures2 = {
      .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
      .pNext = &enableVulkan14Features};

  float queuePriority = 1.0f;

  // create one queue
  VkDeviceQueueCreateInfo queueInfo = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      .queueFamilyIndex = static_cast<uint32_t>(m_GraphicsQueueIndex),
      .queueCount = 1,
      .pQueuePriorities = &queuePriority};

  VkDeviceCreateInfo deviceInfo = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pNext = &enableDeviceFeatures2,
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &queueInfo,
      .enabledExtensionCount = 0};

  VK_CHECK(vkCreateDevice(m_PhysicalDevice, &deviceInfo, nullptr, &m_Device));

  m_DeletionQueue.Push([&] { vkDestroyDevice(m_Device, nullptr); });

  // save the graphics queue
  vkGetDeviceQueue(m_Device, m_GraphicsQueueIndex, 0, &m_GraphicsQueue);
}

void VulkanContext::SetupCommandBuffers() {
  VkCommandPoolCreateInfo poolInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .queueFamilyIndex = m_GraphicsQueueIndex,
  };

  VK_CHECK(vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool));

  m_DeletionQueue.Push(
      [&] { vkDestroyCommandPool(m_Device, m_CommandPool, nullptr); });
}

VulkanContext::~VulkanContext() { m_DeletionQueue.Flush(); }

uint32_t VulkanContext::FindMemoryType(uint32_t typeFilter,
                                       VkMemoryPropertyFlags properties) const {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  // shouldn't get here
  ASSERT(false);
  return 0;
}

VkImageSubresourceRange VulkanContext::ImageSubresourceRange(
    VkImageAspectFlags aspectMask) {
  VkImageSubresourceRange subImage{};
  subImage.aspectMask = aspectMask;
  subImage.baseMipLevel = 0;
  subImage.levelCount = VK_REMAINING_MIP_LEVELS;
  subImage.baseArrayLayer = 0;
  subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

  return subImage;
}

void VulkanContext::TransitionImage(VkCommandBuffer cmd, VkImage image,
                                    VkImageLayout currentLayout,
                                    VkImageLayout newLayout) {
  VkImageMemoryBarrier2 imageBarrier{
      .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
  imageBarrier.pNext = nullptr;

  imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
  imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
  imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
  imageBarrier.dstAccessMask =
      VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

  imageBarrier.oldLayout = currentLayout;
  imageBarrier.newLayout = newLayout;

  VkImageAspectFlags aspectMask =
      (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
          ? VK_IMAGE_ASPECT_DEPTH_BIT
          : VK_IMAGE_ASPECT_COLOR_BIT;
  imageBarrier.subresourceRange = ImageSubresourceRange(aspectMask);
  imageBarrier.image = image;

  VkDependencyInfo depInfo{};
  depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
  depInfo.pNext = nullptr;

  depInfo.imageMemoryBarrierCount = 1;
  depInfo.pImageMemoryBarriers = &imageBarrier;

  vkCmdPipelineBarrier2(cmd, &depInfo);
}

}  // namespace PT
