//
// Created by Stefan on 7/21/2024.
//

#include "core/viewport_panel.hpp"

namespace Viewer {

ViewportPanel::ViewportPanel() : m_RenderedScene(m_Width, m_Height) {
  m_ImageData = new uint32_t[m_Width * m_Height];

  m_PrevWidth = m_Width;
  m_PrevHeight = m_Height;
}

ViewportPanel::~ViewportPanel() { delete[] m_ImageData; }

void ViewportPanel::Render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

  ImGui::Begin("Viewport");

  uint32_t windowWidth = static_cast<int32_t>(ImGui::GetIO().DisplaySize.x);

  m_Width = static_cast<int32_t>(ImGui::GetContentRegionAvail().x);
  m_Height = static_cast<int32_t>(ImGui::GetContentRegionAvail().y);

  // the width check is to avoid rendering on the first frame, before imgui sets up
  // the panel layout
  if((m_Width != m_PrevWidth || m_Height != m_PrevHeight) && m_Width != windowWidth) {
    ResizeScene();
  }

  ImGui::Image(
      (void *)(intptr_t)m_RenderedScene.GetID(),
      ImVec2(static_cast<float>(m_Width), static_cast<float>(m_Height)),
      ImVec2(1, 0), ImVec2(0, 1));

  ImGui::End();

  ImGui::PopStyleVar();

  if(GlobalEventFlags::Get(EventFlag::RenderNow)) {
    RenderScene();
    GlobalEventFlags::Clear(EventFlag::RenderNow);
  }
}

void ViewportPanel::RenderScene() {
  auto captureSpec =
      PT::RenderCaptureSpecification {
          .Width = static_cast<uint32_t>(m_Width),
          .Height = static_cast<uint32_t>(m_Height),
          .Buffer = m_ImageData
      };

  // writes the rendered image data to m_ImageData
  m_Renderer.Capture(captureSpec);

  // updates the opengl texture data
  m_RenderedScene.SetData(m_ImageData);
}

void ViewportPanel::ResizeScene() {
  if(m_PrevWidth * m_PrevHeight < m_Width * m_Height) {
    delete[] m_ImageData;

    m_ImageData = new uint32_t[m_Width * m_Height];
  }

  if(m_PrevWidth * m_PrevHeight == 0) {
    // initial render
    RenderScene();
  }

  // update the image buffer
  m_RenderedScene.Resize(m_Width, m_Height);
  m_RenderedScene.SetData(m_ImageData);

  m_PrevWidth = m_Width;
  m_PrevHeight = m_Height;
}

}  // namespace Viewer
