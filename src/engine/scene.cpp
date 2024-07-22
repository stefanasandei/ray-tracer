//
// Created by Stefan on 7/22/2024.
//

#include "engine/scene.hpp"

namespace PT {

Scene::Scene() = default;

void Scene::Clear() noexcept { m_Geometry.clear(); }

void Scene::Add(const Primitive& shape) noexcept {
  m_Geometry.emplace_back(shape);
}

bool Scene::Hit(const Ray& r, float rayTMin, float rayTMax,
                HitRecord& rec) const {
  HitRecord tempRec{};
  bool hitAnything = false;
  auto closestSoFar = rayTMax;

  for (const auto& shape : m_Geometry) {
    if (shape.Hit(r, rayTMin, closestSoFar, tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.T;
      rec = tempRec;
    }
  }

  return hitAnything;
}

}  // namespace PT