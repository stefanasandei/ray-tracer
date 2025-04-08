//
// Created by Stefan on 4/8/2025.
//

#pragma once

#include "engine/core/base.hpp"
#include "engine/core/deletion_queue.hpp"

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace PT {

class VulkanContext {
 public:
  VulkanContext();
  ~VulkanContext();

  [[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

  [[nodiscard]] VkDevice Device() const { return m_Device; }
  [[nodiscard]] VkQueue GraphicsQueue() const { return m_GraphicsQueue; }
  [[nodiscard]] VkCommandPool CommandPool() const { return m_CommandPool; }
  [[nodiscard]] DeletionQueue& GetDeletionQueue() { return m_DeletionQueue; }

 private:
  void SetupInstance();
  void SetupDevice();
  void SetupCommandBuffers();

 private:
  VkInstance m_Instance;
  VkPhysicalDevice m_PhysicalDevice;
  VkDevice m_Device;

  VkQueue m_GraphicsQueue;
  uint32_t m_GraphicsQueueIndex = -1;

  VkCommandPool m_CommandPool;

  DeletionQueue m_DeletionQueue;
};

}

#define VK_CHECK(x)                                                     \
    do {                                                                \
        VkResult err = x;                                               \
        if (err) {                                                      \
             std::print("Detected Vulkan error: {}\n", string_VkResult(err)); \
            abort();                                                    \
        }                                                               \
    } while (0)
