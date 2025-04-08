//
// Created by Stefan on 4/8/2025.
//

#pragma once

#include "engine/camera.hpp"
#include "engine/scene.hpp"

namespace PT {

// ------ Helper structures ------

struct RenderPayload {
  const Camera& Camera;
  const Scene& Scene;
};

struct RenderCaptureSpecification {
  uint32_t Width, Height;
  uint32_t* Buffer;
};

// ------ Main Backend interface ------

class Backend {
 public:
  virtual void UpdateRenderPayload(const RenderPayload& payload) = 0;
  virtual void RenderToBuffer(const RenderCaptureSpecification& spec) = 0;
};

}
