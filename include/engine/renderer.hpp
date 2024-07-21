//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include <glm/glm.hpp>

#include "engine/camera.hpp"
#include "engine/core/base.hpp"
#include "engine/primitive.hpp"

namespace PT {

// ------ Helper structures ------

struct RenderCaptureSpecification {
  uint32_t Width, Height;
  uint32_t* Buffer;
};

// ------ Main Renderer class ------

class Renderer {
 public:
  Renderer();
  ~Renderer();

  void AddGeometry(const Primitive& primitive);
  void SetActiveCamera(const Camera& camera);

  void Capture(RenderCaptureSpecification& spec);
  static void SaveCapture(const RenderCaptureSpecification& spec,
                          const std::string& filename);

 private:
  [[nodiscard]] glm::vec3 PerPixel(uint32_t x, uint32_t y) const noexcept;

 private:
  Camera m_Camera;
  Primitive m_ScenePrimitive;
};

// ------ Utility function ------

static uint32_t ConvertToRGBA(const glm::vec4& color);

}  // namespace PT
