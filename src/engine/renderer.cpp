//
// Created by Asandei Stefan on 21.07.2024.
//

#include "engine/renderer.hpp"

#include <stb_image_write.h>

#include "engine/ray.hpp"

namespace PT {

Renderer::Renderer()
    : m_Camera({.VerticalFOV = 45.0f,
                .NearClip = 0.1f,
                .FarClip = 100.0f,
                .Width = 1600,
                .Height = 900}) {}

Renderer::~Renderer() = default;

void Renderer::SetGeometry(const Scene& scene) { m_ScenePrimitive = scene; }

void Renderer::SetActiveCamera(const Camera& camera) { m_Camera = camera; }

void Renderer::Capture(RenderCaptureSpecification& spec) {
  m_Camera.Resize(spec.Width, spec.Height);

  spec.Buffer = new uint32_t[spec.Width * spec.Height];

  for (uint32_t y = 0; y < spec.Height; y++) {
    for (uint32_t x = 0; x < spec.Width; x++) {
      glm::vec3 color = PerPixel(x, y);

      spec.Buffer[x + y * spec.Width] = ConvertToRGBA(glm::vec4(color, 1.0f));
    }
  }
}

void Renderer::SaveCapture(const RenderCaptureSpecification& spec,
                           const std::string& filename) {
  constexpr int numChannels = 4;

  stbi_flip_vertically_on_write(1);

  stbi_write_png(filename.c_str(), static_cast<int>(spec.Width),
                 static_cast<int>(spec.Height), numChannels, spec.Buffer,
                 static_cast<int>(spec.Width * sizeof(uint32_t)));
}

glm::vec3 Renderer::PerPixel(uint32_t x, uint32_t y) const noexcept {
  constexpr auto samplesPerPixel = 20;

  glm::vec3 color(0.0f);

  for (auto sample = 0; sample < samplesPerPixel; sample++) {
    auto origin = m_Camera.GetPosition();
    auto direction = m_Camera.GetRayDirections()[x + y * m_Camera.GetWidth()] +
                     Random::Vec3(-0.001f, 0.001f);

    Ray ray(origin, direction);
    color += TraceRay(ray);
  }

  return color / static_cast<float>(samplesPerPixel);
}

glm::vec3 Renderer::TraceRay(const PT::Ray& ray,
                             uint32_t depth) const noexcept {
  if (depth <= 0) return glm::vec3(0.0f);

  HitRecord rec{};
  if (m_ScenePrimitive.Hit(ray, 0.001, std::numeric_limits<float>::infinity(),
                           rec)) {
    glm::vec3 direction = rec.Normal + Random::InUnitSphere();
    return 0.5f * TraceRay(Ray(rec.Point, direction), depth - 1);
  }

  return glm::vec3(1.0f);
}

static uint32_t ConvertToRGBA(const glm::vec4& color) {
  auto r = (uint8_t)(color.r * 255.0f);
  auto g = (uint8_t)(color.g * 255.0f);
  auto b = (uint8_t)(color.b * 255.0f);
  auto a = (uint8_t)(color.a * 255.0f);

  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

}  // namespace PT
