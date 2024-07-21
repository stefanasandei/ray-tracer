//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include <glm/glm.hpp>

#include "engine/core/base.hpp"

namespace PT {

class Ray {
 public:
  Ray(const glm::vec3& origin, const glm::vec3& dir)
      : m_Origin(origin), m_Direction(dir) {}
  ~Ray() = default;

  [[nodiscard]] glm::vec3 At(float t) const {
    return m_Origin + t * m_Direction;
  }

  [[nodiscard]] const glm::vec3& GetOrigin() const { return m_Origin; }
  [[nodiscard]] const glm::vec3& GetDirection() const { return m_Direction; }

 private:
  glm::vec3 m_Origin, m_Direction;
};

}  // namespace PT
