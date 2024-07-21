//
// Created by Asandei Stefan on 20.07.2024.
//

#pragma once

#include <glad/glad.h>

#include "core/base.hpp"
#include "gfx/ui_layer.hpp"

namespace Viewer {

// ---- Helper Structs ----

// ---- The Graphics Context ----

class GfxContext {
 public:
  explicit GfxContext();
  ~GfxContext();

  void RenderFrame() noexcept;

 private:
  uint32_t m_FrameIndex = 0;
};

}  // namespace Viewer
