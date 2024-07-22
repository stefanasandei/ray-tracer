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

 private:
  glm::vec3 m_Origin;
  float m_Radius;
};

}  // namespace PT
