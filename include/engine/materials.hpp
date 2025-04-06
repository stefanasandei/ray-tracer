//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include "engine/base/material.hpp"
#include "engine/core/base.hpp"

namespace PT {

class Lambertian : public Material {
 public:
  explicit Lambertian(const glm::vec3& albedo);

  bool Scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
  [[nodiscard]] const glm::vec3& GetAlbedo() const { return m_Albedo; }

 private:
  glm::vec3 m_Albedo;
};

class Metal : public Material {
 public:
  Metal(const glm::vec3& albedo, float fuzz);

  bool Scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation,
               Ray& scattered) const override;

  void SetAlbedo(const glm::vec3& albedo) { m_Albedo = albedo; }
  [[nodiscard]] const glm::vec3& GetAlbedo() const { return m_Albedo; }

  void SetFuzz(const float& fuzz) { m_Fuzz = fuzz; }
  [[nodiscard]] const float& GetFuzz() const { return m_Fuzz; }

 private:
  glm::vec3 m_Albedo;
  float m_Fuzz;
};

}  // namespace PT
