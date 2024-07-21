//
// Created by Stefan on 7/20/2024.
//

#pragma once

#include <imgui.h>

namespace Viewer {

class UILayer {
 public:
  UILayer() {}
  ~UILayer() {}

  void RenderUI() noexcept {}

  void BeginUI() noexcept {}
  void EndUI() noexcept {}

 private:
};

}  // namespace Viewer
