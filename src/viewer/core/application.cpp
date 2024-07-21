//
// Created by Stefan on 7/20/2024.
//

#include "application.hpp"

namespace Viewer {

static uint32_t FrameIndex = 0;

Application::Application() {
  constexpr int32_t width = 1600, height = 900;

  m_Window = std::make_shared<Window>(WindowSpecification{
      .Width = width, .Height = height, .Title = "Path Tracer Viewer"});

  m_GfxContext = std::make_shared<GfxContext>(GfxContextSpecification{
      .GFLWHandle = m_Window->GetHandle(), .Width = width, .Height = height});
}

Application::~Application() = default;

int32_t Application::Run() noexcept {
  while (!m_Window->ShouldClose()) {
    m_Window->Update();
    m_GfxContext->RenderFrame();
  }

  return 0;
}

}  // namespace Viewer
