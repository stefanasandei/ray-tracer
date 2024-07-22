//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include "engine/base/material.hpp"
#include "engine/core/base.hpp"

namespace PT {

class Lambertian : public Material {
 public:
  Lambertian(const glm::vec3& albedo);

  bool Scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

 private:
  glm::vec3 m_Albedo;
};

}  // namespace PT
