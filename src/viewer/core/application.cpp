//
// Created by Stefan on 7/20/2024.
//

#include "application.hpp"

namespace Viewer {

Application::Application() {
  constexpr int32_t width = 1600, height = 900;

  m_Window = std::make_shared<Window>(WindowSpecification{
      .Width = width, .Height = height, .Title = "Path Tracer Viewer"});

  m_GfxContext = std::make_shared<GfxContext>();

  m_UILayer = std::make_shared<UILayer>(m_Window->GetHandle());
}

Application::~Application() = default;

int32_t Application::Run() noexcept {
  while (!m_Window->ShouldClose()) {
    m_Window->Update();
    m_GfxContext->RenderFrame();

    m_UILayer->BeginUI();
    {
      ImGui::Begin("test");
      ImGui::Text("hello world");
      ImGui::End();
    }
    m_UILayer->EndUI();
  }

  return 0;
}

}  // namespace Viewer
