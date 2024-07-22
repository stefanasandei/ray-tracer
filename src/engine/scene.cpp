//
// Created by Stefan on 7/22/2024.
//

#include "engine/scene.hpp"

namespace PT {

Scene::Scene() = default;

void Scene::Clear() noexcept { m_Shapes.clear(); }

void Scene::Add(const std::shared_ptr<Shape>& shape) noexcept {
  m_Shapes.emplace_back(shape);
}

bool Scene::Hit(const Ray& r, float rayTMin, float rayTMax,
                HitRecord& rec) const {
  HitRecord tempRec{};
  bool hitAnything = false;
  auto closestSoFar = rayTMax;

  for (const auto& shape : m_Shapes) {
    if (shape->Hit(r, rayTMin, closestSoFar, tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.T;
      rec = tempRec;
    }
  }

  return hitAnything;
}

}  // namespace PT