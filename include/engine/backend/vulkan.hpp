//
// Created by Stefan on 4/8/2025.
//

//
// Created by Stefan on 4/8/2025.
//

#pragma once

#include <glm/glm.hpp>

#include "engine/backend/backend.hpp"
#include "engine/backend/vk/context.hpp"
#include "engine/camera.hpp"
#include "engine/core/base.hpp"
#include "engine/scene.hpp"

namespace PT {

class VulkanBackend : public virtual Backend {
 public:
  VulkanBackend();
  ~VulkanBackend();

  void UpdateRenderPayload(const RenderPayload& payload) override;
  void RenderToBuffer(const RenderCaptureSpecification& spec) override;

 private:
  void InitPerFrameResources(uint32_t width, uint32_t height);

 private:
  VulkanContext m_Context;

  VkImage m_Image = VK_NULL_HANDLE;
  VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
  VkCommandBuffer m_Cmd = VK_NULL_HANDLE;

  const Camera* m_Camera{};
  const Scene* m_ScenePrimitive{};

  uint32_t m_CurrentWidth{0}, m_CurrentHeight{0};
};

}  // namespace PT
