//
// Created by Stefan on 4/8/2025.
//

#include "engine/backend/vulkan.hpp"

namespace PT {

VulkanBackend::VulkanBackend() = default;

VulkanBackend::~VulkanBackend() = default;

void VulkanBackend::UpdateRenderPayload(const RenderPayload& payload) {
  m_Camera = &payload.Camera;
  m_ScenePrimitive = &payload.Scene;
}

void VulkanBackend::RenderToBuffer(const RenderCaptureSpecification& spec) {
  std::fill(spec.Buffer, spec.Buffer + spec.Width * spec.Height, 0xFF0000FF);
}

}  // namespace PT
