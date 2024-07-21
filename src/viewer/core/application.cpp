//
// Created by Stefan on 7/20/2024.
//

#include "core/application.hpp"

#include "core/settings_panel.hpp"
#include "core/viewport_panel.hpp"

namespace Viewer {

Application::Application() {
  constexpr int32_t width = 1600, height = 900;

  m_Window = std::make_shared<Window>(WindowSpecification{
      .Width = width, .Height = height, .Title = "Path Tracer Viewer"});

  m_GfxContext = std::make_shared<GfxContext>();

  m_UILayer = std::make_shared<UILayer>(m_Window->GetHandle());

  m_Panels.push_back(std::make_shared<SettingsPanel>());
  m_Panels.push_back(std::make_shared<ViewportPanel>());
}

Application::~Application() = default;

int32_t Application::Run() noexcept {
  while (!m_Window->ShouldClose()) {
    m_Window->Update();
    m_GfxContext->RenderFrame();

    m_UILayer->BeginUI();
    {
      ShowUI();
    }
    m_UILayer->EndUI();
  }

  return 0;
}

void Application::ShowUI() noexcept {
  for (auto& panel : m_Panels) {
    panel->Render();
  }
}

}  // namespace Viewer
