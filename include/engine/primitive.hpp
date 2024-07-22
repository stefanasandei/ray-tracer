//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include "engine/base/material.hpp"
#include "engine/base/shape.hpp"
#include "engine/core/base.hpp"

namespace PT {

class Primitive {
 public:
  Primitive(const std::shared_ptr<Shape>& shape,
            const std::shared_ptr<Material>& material);
  ~Primitive();

  void SetShape(const std::shared_ptr<Shape>& shape);
  void SetMaterial(const std::shared_ptr<Material>& material);

  [[nodiscard]] std::shared_ptr<Shape> GetShape() const;
  [[nodiscard]] std::shared_ptr<Material> GetMaterial() const;

  bool Hit(const Ray& r, float rayTMin, float rayTMax, HitRecord& rec) const;

 private:
  std::shared_ptr<Shape> m_Shape;
  std::shared_ptr<Material> m_Material;
};

}  // namespace PT
