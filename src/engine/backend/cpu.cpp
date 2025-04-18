//
// Created by Stefan on 4/8/2025.
//

#include "engine/backend/cpu.hpp"

#include <algorithm>
#include <execution>

namespace PT {

CPUBackend::CPUBackend() : m_Camera(nullptr), m_ScenePrimitive(nullptr) {}

CPUBackend::~CPUBackend() = default;

void CPUBackend::UpdateRenderPayload(const RenderPayload& payload) {
  m_Camera = &payload.Camera;
  m_ScenePrimitive = &payload.Scene;
}

void CPUBackend::RenderToBuffer(const RenderCaptureSpecification& spec) {
  std::vector<int> heightIter(spec.Height);
  for (int i = 0; i < spec.Height; i++) heightIter[i] = i;

  std::vector<int> widthIter(spec.Width);
  for (int i = 0; i < spec.Width; i++) widthIter[i] = i;

  std::for_each(std::execution::par, heightIter.begin(), heightIter.end(),
                [this, &widthIter, &spec](uint32_t y) {
                  std::for_each(std::execution::seq, widthIter.begin(),
                                widthIter.end(), [this, &y, &spec](uint32_t x) {
                                  glm::vec3 color = PerPixel(x, y);

                                  spec.Buffer[x + y * spec.Width] =
                                      ConvertToRGBA(glm::vec4(color, 1.0f));
                                });
                });
}

glm::vec3 CPUBackend::PerPixel(uint32_t x, uint32_t y) const noexcept {
  glm::vec3 color(0.0f);

  for (uint32_t sample = 0; sample < m_Camera->GetSamplesPerPixel(); sample++) {
    auto origin = m_Camera->GetPosition();
    auto direction =
        m_Camera->GetRayDirections()[x + y * m_Camera->GetWidth()] +
        Random::Vec3(-0.001f, 0.001f);

    Ray ray(origin, direction);
    color += TraceRay(ray);
  }

  return color / static_cast<float>(m_Camera->GetSamplesPerPixel());
}

glm::vec3 CPUBackend::TraceRay(const PT::Ray& ray,
                               uint32_t depth) const noexcept {
  if (depth <= 0) return glm::vec3(0.0f);

  HitRecord rec{};
  if (m_ScenePrimitive->Hit(ray, 0.001, std::numeric_limits<float>::infinity(),
                            rec)) {
    Ray scattered(glm::vec3(0.0f), glm::vec3(0.0f));
    glm::vec3 attenuation(0.0f);
    if (rec.Mat->Scatter(ray, rec, attenuation, scattered))
      return attenuation * TraceRay(scattered, depth - 1);
    return glm::vec3(0.0f);
  }

  auto a =
      glm::vec3(0.5) * (glm::normalize(ray.GetDirection()) + glm::vec3(1.0));
  return (glm::vec3(1.0) - a) * glm::vec3(1.0, 1.0, 1.0) +
         a * glm::vec3(0.5, 0.7, 1.0);
}

}  // namespace PT