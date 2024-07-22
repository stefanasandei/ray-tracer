//
// Created by Asandei Stefan on 21.07.2024.
//

#include "engine/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace PT {

Camera::Camera(const CameraSpecification& spec)
    : m_VerticalFOV(spec.VerticalFOV),
      m_NearClip(spec.NearClip),
      m_FarClip(spec.FarClip),
      m_ViewportWidth(spec.Width),
      m_ViewportHeight(spec.Height) {
  m_ForwardDirection = glm::vec3(0, 0, -1);
  m_Position = glm::vec3(0, 0, 3);

  RecalculateProjection();
  RecalculateView();
  RecalculateRayDirections();
}

Camera::~Camera() = default;

void Camera::Resize(uint32_t width, uint32_t height) {
  m_ViewportWidth = width;
  m_ViewportHeight = height;

  RecalculateProjection();
  RecalculateRayDirections();
}

void Camera::RecalculateProjection() {
  m_Projection =
      glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)m_ViewportWidth,
                          (float)m_ViewportHeight, m_NearClip, m_FarClip);
}

void Camera::RecalculateView() {
  m_View = glm::lookAt(m_Position, m_Position + m_ForwardDirection,
                       glm::vec3(0, 1, 0));
}

void Camera::RecalculateRayDirections() {
  m_RayDirections.resize(m_ViewportWidth * m_ViewportHeight);

  auto inverseProjection = glm::inverse(m_Projection);
  auto inverseView = glm::inverse(m_View);

  for (uint32_t y = 0; y < m_ViewportHeight; y++) {
    for (uint32_t x = 0; x < m_ViewportWidth; x++) {
      glm::vec2 coord = {(float)x / (float)m_ViewportWidth,
                         (float)y / (float)m_ViewportHeight};
      coord = coord * 2.0f - 1.0f;

      glm::vec4 target = inverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
      glm::vec3 rayDirection =
          glm::vec3(inverseView *
                    glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));
      m_RayDirections[x + y * m_ViewportWidth] = rayDirection;
    }
  }
}

}  // namespace PT
