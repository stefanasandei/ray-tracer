//
// Created by Stefan on 7/22/2024.
//

#pragma once

#include "engine/base/shape.hpp"
#include "engine/primitive.hpp"

namespace PT {

class Scene : public Shape {
 public:
  Scene();

  void Clear() noexcept;
  void Add(const Primitive& shape) noexcept;

  bool Hit(const Ray& r, float rayTMin, float rayTMax,
           HitRecord& rec) const override;

 private:
  std::vector<Primitive> m_Geometry;
};

}  // namespace PT
