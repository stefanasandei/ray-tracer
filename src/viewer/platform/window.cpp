//
// Created by Stefan on 7/20/2024.
//

#include "platform/window.hpp"

namespace Viewer {

Window::Window(const WindowSpecification &spec) {
  if (!glfwInit()) std::exit(0);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  m_Window = glfwCreateWindow(static_cast<int>(spec.Width),
                              static_cast<int>(spec.Height), spec.Title.c_str(),
                              nullptr, nullptr);
  if (!m_Window) {
    glfwTerminate();
    std::exit(0);
  }

  glfwMakeContextCurrent(m_Window);
}

Window::~Window() { glfwTerminate(); }

bool Window::ShouldClose() const { return glfwWindowShouldClose(m_Window); }

void Window::Update() const {
  glfwPollEvents();
  glfwSwapBuffers(m_Window);
}

}  // namespace Viewer
