//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include "engine/core/base.hpp"
#include "engine/ray.hpp"

namespace PT {

struct HitRecord;

class Material {
 public:
  virtual ~Material() = default;

  virtual bool Scatter(const Ray& ray, const HitRecord& rec,
                       glm::vec3& attenuation, Ray& scattered) const {
    return false;
  }
};

}  // namespace PT
