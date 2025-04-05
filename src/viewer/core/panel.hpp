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
};

extern EventFlags GlobalEventFlags;

class Panel {
 public:
  Panel() = default;
  ~Panel() = default;

  virtual void Render() {}
};

}  // namespace Viewer