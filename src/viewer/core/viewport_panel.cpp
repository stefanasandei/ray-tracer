//
// Created by Stefan on 7/21/2024.
//

#include "core/viewport_panel.hpp"

namespace Viewer {

ViewportPanel::ViewportPanel() : m_RenderedScene(m_Width, m_Height) {}

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

void ViewportPanel::RenderScene() {
  m_RenderedScene.Resize(m_Width, m_Height);

  delete[] m_ImageData;
  m_ImageData = new uint8_t[m_Width * m_Height * 3];

  for (int i = 0; i < m_Width * m_Height * 3; i += 3) {
    int y = (i / 3) / m_Width;
    int x = (i / 3) % m_Width;
    m_ImageData[i + 0] = (float)x / (m_Width) * 255;
    m_ImageData[i + 1] = (float)y / (m_Height) * 255;
    m_ImageData[i + 2] = 0 * 255;
  }

  m_RenderedScene.SetData(m_ImageData);
}

}  // namespace Viewer
