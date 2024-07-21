//
// Created by Stefan on 7/20/2024.
//

#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Viewer {

class UILayer {
 public:
  explicit UILayer(GLFWwindow* glfwHandle);
  ~UILayer();

  void BeginUI() noexcept;
  void EndUI() noexcept;

 private:
  bool m_Open = true;
};

}  // namespace Viewer
