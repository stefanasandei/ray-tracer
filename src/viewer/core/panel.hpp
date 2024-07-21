//
// Created by Stefan on 7/21/2024.
//

#pragma once

#include <imgui.h>

#include "core/base.hpp"

namespace Viewer {

class Panel {
 public:
  Panel() = default;
  ~Panel() = default;

  virtual void Render() {}
};

}  // namespace Viewer