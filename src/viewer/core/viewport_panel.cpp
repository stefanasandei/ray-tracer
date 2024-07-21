//
// Created by Stefan on 7/21/2024.
//

#include "core/viewport_panel.hpp"

namespace Viewer {

ViewportPanel::ViewportPanel() : m_RenderedScene(m_Width, m_Height) {
  auto captureSpec =
      PT::RenderCaptureSpecification{.Width = static_cast<uint32_t>(m_Width),
                                     .Height = static_cast<uint32_t>(m_Height)};
  m_Renderer.Capture(captureSpec);

  m_ImageData = captureSpec.Buffer;
  m_RenderedScene.SetData(captureSpec.Buffer);
}

ViewportPanel::~ViewportPanel() { delete[] m_ImageData; }

void ViewportPanel::Render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

  ImGui::Begin("Viewport");

  m_Width = static_cast<int32_t>(ImGui::GetContentRegionAvail().x);
  m_Height = static_cast<int32_t>(ImGui::GetContentRegionAvail().y);

  if (m_ImageData == nullptr) {
    RenderScene();
  }

  ImGui::Image(
      (void *)(intptr_t)m_RenderedScene.GetID(),
      ImVec2(static_cast<float>(m_Width), static_cast<float>(m_Height)),
      ImVec2(1, 0), ImVec2(0, 1));

  ImGui::End();

  ImGui::PopStyleVar();
}

void ViewportPanel::RenderScene() {}

}  // namespace Viewer
