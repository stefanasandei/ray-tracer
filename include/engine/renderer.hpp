//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include "engine/core/base.hpp"

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

  void AddGeometry(/* Geometry objects */);
  void SetActiveCamera(/* Camera Object */);

  void Capture(RenderCaptureSpecification& spec) const;
  void SaveCapture(const RenderCaptureSpecification& spec,
                   const std::string& filename) const;
};

}  // namespace PT
