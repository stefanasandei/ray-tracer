//
// Created by Asandei Stefan on 21.07.2024.
//

#include "engine/materials.hpp"

#include "engine/base/shape.hpp"

namespace PT {

Lambertian::Lambertian(const glm::vec3& albedo) : m_Albedo(albedo) {}

bool Lambertian::Scatter(const Ray& ray, const HitRecord& rec,
                         glm::vec3& attenuation, Ray& scattered) const {
  auto scatterDirection = rec.Normal + Random::InUnitSphere();
  scattered = Ray(rec.Point, scatterDirection);
  attenuation = m_Albedo;
  return true;
}

}  // namespace PT