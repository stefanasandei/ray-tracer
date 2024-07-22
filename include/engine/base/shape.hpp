//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include "engine/core/base.hpp"
#include "engine/ray.hpp"

namespace PT {

struct HitRecord {
  glm::vec3 Point;
  glm::vec3 Normal;
  float T;
  bool IsFrontFace;

  void SetFaceNormal(const Ray& r, const glm::vec3& outwardNormal) {
    IsFrontFace = glm::dot(r.GetDirection(), outwardNormal) < 0;
    Normal = IsFrontFace ? outwardNormal : -outwardNormal;
  }
};

class Shape {
 public:
  virtual ~Shape() = default;

  virtual bool Hit(const Ray& r, float rayTMin, float rayTMax,
                   HitRecord& rec) const = 0;
};

}  // namespace PT
