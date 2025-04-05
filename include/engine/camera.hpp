//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include <glm/glm.hpp>

#include "engine/core/base.hpp"

namespace PT {

struct CameraSpecification {
  float VerticalFOV;
  float NearClip;
  float FarClip;
  uint32_t SamplesPerPixel;
  uint32_t Width, Height;
};

class Camera {
 public:
  explicit Camera(const CameraSpecification& spec);
  ~Camera();

  [[nodiscard]] const uint32_t& GetSamplesPerPixel() const { return m_SamplesPerPixel; }

  [[nodiscard]] const glm::mat4& GetProjection() const { return m_Projection; }
  [[nodiscard]] const glm::mat4& GetView() const { return m_View; }
  [[nodiscard]] const std::vector<glm::vec3>& GetRayDirections() const {
    return m_RayDirections;
  }
  [[nodiscard]] const uint32_t& GetWidth() const { return m_ViewportWidth; }
  [[nodiscard]] const uint32_t& GetHeight() const { return m_ViewportHeight; }
  [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
  [[nodiscard]] const glm::vec3& GetDirection() const {
    return m_ForwardDirection;
  }

  void Resize(uint32_t width, uint32_t height);

 private:
  void RecalculateProjection();
  void RecalculateView();
  void RecalculateRayDirections();

 private:
  glm::mat4 m_Projection{1.0f};
  glm::mat4 m_View{1.0f};

  float m_VerticalFOV = 45.0f;
  float m_NearClip = 0.1f;
  float m_FarClip = 100.0f;

  uint32_t m_SamplesPerPixel = 20;

  glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
  glm::vec3 m_ForwardDirection{0.0f, 0.0f, 0.0f};

  std::vector<glm::vec3> m_RayDirections;

  uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};

}  // namespace PT
