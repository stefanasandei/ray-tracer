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
  VulkanContext m_Context;

  const Camera* m_Camera{};
  const Scene* m_ScenePrimitive{};
};

}  // namespace PT
