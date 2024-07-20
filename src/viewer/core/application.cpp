//
// Created by Stefan on 7/20/2024.
//

#include "application.hpp"

namespace Viewer {

Application::Application() {
  m_Window = std::make_shared<Window>(WindowSpecification{
      .Width = 1600, .Height = 900, .Title = "Path Tracer Viewer"});
}

Application::~Application() = default;

int32_t Application::Run() noexcept {
  while (!m_Window->ShouldClose()) {
    m_Window->Update();
  }

  return 0;
}

}  // namespace Viewer
