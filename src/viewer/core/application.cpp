//
// Created by Stefan on 7/20/2024.
//

#include "application.hpp"

namespace Viewer {

static uint32_t FrameIndex = 0;

Application::Application() {
  m_Window = std::make_shared<Window>(WindowSpecification{
      .Width = 1080, .Height = 720, .Title = "Path Tracer Viewer"});

  m_GfxContext = std::make_shared<GfxContext>(GfxContextSpecification{
      .GFLWHandle = m_Window->GetHandle(), .Width = 1080, .Height = 720});
}

Application::~Application() = default;

int32_t Application::Run() noexcept {
  while (!m_Window->ShouldClose()) {
    m_Window->Update();
    m_GfxContext->RenderFrame(FrameIndex++);
  }

  return 0;
}

}  // namespace Viewer
