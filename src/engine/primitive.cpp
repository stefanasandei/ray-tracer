//
// Created by Asandei Stefan on 21.07.2024.
//

#include "engine/primitive.hpp"

namespace PT {

Primitive::Primitive(const std::shared_ptr<Shape>& shape,
                     const std::shared_ptr<Material>& material)
    : m_Shape(shape), m_Material(material) {}

Primitive::~Primitive() = default;

void Primitive::SetShape(const std::shared_ptr<Shape>& shape) {
  m_Shape = shape;
}

void Primitive::SetMaterial(const std::shared_ptr<Material>& material) {
  m_Material = material;
}

std::shared_ptr<Shape> Primitive::GetShape() const { return m_Shape; }

std::shared_ptr<Material> Primitive::GetMaterial() const { return m_Material; }

bool Primitive::Hit(const Ray& r, float rayTMin, float rayTMax,
                    HitRecord& rec) const {
  rec.Mat = m_Material;
  return m_Shape->Hit(r, rayTMin, rayTMax, rec);
}

}  // namespace PT