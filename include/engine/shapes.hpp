//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include "engine/base/shape.hpp"
#include "engine/core/base.hpp"

namespace PT {

class Sphere : public Shape {
 public:
  Sphere(const glm::vec3& origin, float radius);

  bool Hit(const Ray& r, float rayTMin, float rayTMax,
           HitRecord& rec) const override;

  void SetOrigin(const glm::vec3& origin) { m_Origin = origin; }
  [[nodiscard]] const glm::vec3& GetOrigin() const { return m_Origin; }

  void SetRadius(const float& radius) { m_Radius = radius; }
  [[nodiscard]] const float& GetRadius() const { return m_Radius; }

 private:
  glm::vec3 m_Origin;
  float m_Radius;
};

}  // namespace PT
