//
// Created by Asandei Stefan on 20.07.2024.
//

#pragma once

#include <glad/glad.h>

#include "core/base.hpp"
#include "gfx/ui_layer.hpp"

namespace Viewer {

// ---- Helper Structs ----

struct GfxContextSpecification {
  void *GFLWHandle;
  int32_t Width, Height;
};

// ---- The Graphics Context ----

class GfxContext {
 public:
  explicit GfxContext(const GfxContextSpecification &spec);
  ~GfxContext();

  void RenderFrame() noexcept;

 private:
 private:
  std::shared_ptr<UILayer> m_UILayer;
};

}  // namespace Viewer
