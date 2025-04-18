//
// Created by Stefan on 7/20/2024.
//

#include "platform/window.hpp"

namespace Viewer {

Window::Window(const WindowSpecification &spec) {
  if (!glfwInit()) std::exit(0);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_Window = glfwCreateWindow(static_cast<int>(spec.Width),
                              static_cast<int>(spec.Height), spec.Title.c_str(),
                              nullptr, nullptr);
  if (!m_Window) {
    glfwTerminate();
    std::exit(0);
  }

  glfwMakeContextCurrent(m_Window);
  glfwSwapInterval(0);
}

Window::~Window() { glfwTerminate(); }

bool Window::ShouldClose() const { return glfwWindowShouldClose(m_Window); }

void Window::Update() const {
  glfwPollEvents();
  glfwSwapBuffers(m_Window);
}

}  // namespace Viewer
