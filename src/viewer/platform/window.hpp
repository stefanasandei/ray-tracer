//
// Created by Stefan on 7/20/2024.
//

#pragma once

#include <GLFW/glfw3.h>

#include "core/base.hpp"

namespace Viewer {

struct WindowSpecification {
  uint32_t Width, Height;
  std::string Title;
};

class Window {
 public:
  explicit Window(const WindowSpecification &spec);
  ~Window();

  [[nodiscard]] bool ShouldClose() const;
  void Update() const;

  [[nodiscard]] GLFWwindow *GetHandle() const { return m_Window; }

 private:
  GLFWwindow *m_Window;
};

}  // namespace Viewer
