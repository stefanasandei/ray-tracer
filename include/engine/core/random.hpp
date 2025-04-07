//
// Created by Asandei Stefan on 22.07.2024.
//

#pragma once

#include <glm/glm.hpp>
#include <random>

namespace PT {

class Random {
 public:
  static void Init() {}

  static uint32_t UInt() {
    // PCG Hash
    // https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/#hash-or-prng

    uint32_t state = RngState;
    RngState = RngState * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
  }

  static uint32_t UInt(uint32_t min, uint32_t max) {
    return min + (UInt() % (max - min + 1));
  }

  static float Float() {
    return (float)UInt() / (float)std::numeric_limits<uint32_t>::max();
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
  static uint32_t RngState;
};

}  // namespace PT
