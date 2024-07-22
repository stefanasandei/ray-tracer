//
// Created by Asandei Stefan on 21.07.2024.
//

#include "engine/shapes.hpp"

namespace PT {

Sphere::Sphere(const glm::vec3& origin, float radius)
    : m_Origin(origin), m_Radius(radius) {}

bool Sphere::Hit(const Ray& r, float rayTMin, float rayTMax,
                 HitRecord& rec) const {
  auto oc = m_Origin - r.GetOrigin();
  auto a = glm::length(r.GetDirection()) * glm::length(r.GetDirection());
  auto h = glm::dot(r.GetDirection(), oc);
  auto c = glm::length(oc) * glm::length(oc) - m_Radius * m_Radius;

  auto discriminant = h * h - a * c;
  if (discriminant < 0) return false;

  auto sqrtd = glm::sqrt(discriminant);

  auto root = (h - sqrtd) / a;
  if (root <= rayTMin || rayTMax <= root) {
    root = (h + sqrtd) / a;
    if (root <= rayTMin || rayTMax <= root) {
      return false;
    }
  }

  rec.T = root;
  rec.Point = r.At(rec.T);
  glm::vec3 outwardNormal = (rec.Point - m_Origin) / m_Radius;
  rec.SetFaceNormal(r, outwardNormal);

  return true;
}

}  // namespace PT