//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma message("Change the Renderer API from ./include/renderer.hpp. The CPU backend fully works, while the Vulkan backend is a work in progress.")

#include "engine/renderer.hpp"

#include <stb_image_write.h>

#include "engine/backend/cpu.hpp"
#include "engine/backend/vulkan.hpp"
#include "engine/ray.hpp"

namespace PT {

Renderer::Renderer(RendererAPI api)
    : m_Camera({.VerticalFOV = 45.0f,
                .NearClip = 0.1f,
                .FarClip = 100.0f,
                .SamplesPerPixel = 1,
                .Width = 0,
                .Height = 0}) {
  m_API = api;

  switch (m_API) {
    case RendererAPI::CPU:
      m_Backend = std::make_shared<CPUBackend>();
      break;
    case RendererAPI::VULKAN:
      m_Backend = std::make_shared<VulkanBackend>();
    default:
      break;
  }
}

Renderer::~Renderer() = default;

void Renderer::SetGeometry(const Scene& scene) {
  m_ScenePrimitive = scene;

  m_Backend->UpdateRenderPayload({m_Camera, m_ScenePrimitive});
}

void Renderer::SetActiveCamera(const Camera& camera) {
  m_Camera = camera;

  m_Backend->UpdateRenderPayload({m_Camera, m_ScenePrimitive});
}

void Renderer::Capture(RenderCaptureSpecification& spec) {
  m_Camera.Resize(spec.Width, spec.Height);

  if (spec.Buffer == nullptr) {
    spec.Buffer = new uint32_t[spec.Width * spec.Height];
  }

  m_Backend->RenderToBuffer(spec);
}

void Renderer::SaveCapture(const RenderCaptureSpecification& spec,
                           const std::string& filename) {
  constexpr int numChannels = 4;

  stbi_flip_vertically_on_write(1);

  stbi_write_png(filename.c_str(), static_cast<int>(spec.Width),
                 static_cast<int>(spec.Height), numChannels, spec.Buffer,
                 static_cast<int>(spec.Width * sizeof(uint32_t)));
}

}  // namespace PT
