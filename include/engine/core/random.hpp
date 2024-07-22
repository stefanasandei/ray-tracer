//
// Created by Asandei Stefan on 22.07.2024.
//

#pragma once

#include <glm/glm.hpp>
#include <random>

namespace PT {

class Random {
 public:
  static void Init() { s_RandomEngine.seed(std::random_device()()); }

  static uint32_t UInt() { return s_Distribution(s_RandomEngine); }

  static uint32_t UInt(uint32_t min, uint32_t max) {
    return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
  }

  static float Float() {
    return (float)s_Distribution(s_RandomEngine) /
           (float)std::numeric_limits<uint32_t>::max();
  }

  static glm::vec3 Vec3() { return {Float(), Float(), Float()}; }

  static glm::vec3 Vec3(float min, float max) {
    return {Float() * (max - min) + min, Float() * (max - min) + min,
            Float() * (max - min) + min};
  }

  static glm::vec3 InUnitSphere() { return glm::normalize(Vec3(-1.0f, 1.0f)); }

  static glm::vec3 InHemisphere(const glm::vec3& normal) {
    glm::vec3 onUnitSphere = InUnitSphere();
    if (glm::dot(onUnitSphere, normal) > 0.0) return onUnitSphere;
    return -onUnitSphere;
  }

 private:
  static std::mt19937 s_RandomEngine;
  static std::uniform_int_distribution<std::mt19937::result_type>
      s_Distribution;
};

}  // namespace PT
