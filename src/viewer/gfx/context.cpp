//
// Created by Asandei Stefan on 20.07.2024.
//

#include "gfx/context.hpp"

#include <GLFW/glfw3.h>

namespace Viewer {

GfxContext::GfxContext() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) std::exit(1);
}

GfxContext::~GfxContext() = default;

void GfxContext::RenderFrame() noexcept {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_FrameIndex++;
}

}  // namespace Viewer
