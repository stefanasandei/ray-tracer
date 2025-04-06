//
// Created by Stefan on 7/21/2024.
//

#pragma once

#include <imgui.h>

#include "core/base.hpp"

namespace Viewer {

// used for panels to send messages to each other
struct EventFlags {
  bool RenderNow = false;
  bool SceneUpdated = false;
};

struct PanelState {
  int32_t DownsampleFactor = 10;
  uint32_t* SamplesPerPixel = nullptr;

  uint32_t ActivePrimitiveIdx = 1;
  PT::Scene Scene;
};

extern EventFlags GlobalEventFlags;
extern PanelState GlobalPanelState;

class Panel {
 public:
  Panel() = default;
  ~Panel() = default;

  virtual void Render() {}
};

}  // namespace Viewer