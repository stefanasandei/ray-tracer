//
// Created by Stefan on 7/21/2024.
//

#include "panels/viewport_panel.hpp"

namespace Viewer {

ViewportPanel::ViewportPanel() : m_RenderedScene(m_Width, m_Height) {
  // we do this instead of allocating the exact width x height and resizing for
  // changes to avoid memory reallocation
  constexpr uint32_t MAX_WIDTH = 3840, MAX_HEIGHT = 2160;  // 4k res
  m_ImageData.resize(MAX_WIDTH * MAX_HEIGHT);

  m_PrevWidth = m_Width;
  m_PrevHeight = m_Height;

  GlobalPanelState.SamplesPerPixel =
      m_Renderer.GetCamera().GetSamplesPerPixel();
}

void ViewportPanel::Render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

  ImGui::Begin("Viewport");

  uint32_t windowWidth = static_cast<int32_t>(ImGui::GetIO().DisplaySize.x);

  m_Width = static_cast<int32_t>(ImGui::GetContentRegionAvail().x);
  m_Height = static_cast<int32_t>(ImGui::GetContentRegionAvail().y);

  // the width check is to avoid rendering on the first frame, before imgui sets
  // up the panel layout
  if ((m_Width != m_PrevWidth || m_Height != m_PrevHeight) &&
      m_Width != windowWidth) {
    ResizeScene();
  }

  ImGui::Image(
      (void *)(intptr_t)m_RenderedScene.GetID(),
      ImVec2(static_cast<float>(m_Width), static_cast<float>(m_Height)),
      ImVec2(0, 1), ImVec2(1, 0));

  ImGui::End();

  ImGui::PopStyleVar();

  // handle events from other panels
  HandlePanelEvents();
}

void ViewportPanel::HandlePanelEvents() {
  if (GlobalEventFlags.RenderNow) {
    RenderScene();
    GlobalEventFlags.RenderNow = false;
  }

  if (GlobalEventFlags.SceneUpdated) {
    m_Renderer.SetGeometry(GlobalPanelState.Scene);
    GlobalEventFlags.SceneUpdated = false;
  }

  if (GlobalEventFlags.ExportToImage) {
    auto captureSpec = PT::RenderCaptureSpecification{
        .Width =
            static_cast<uint32_t>(m_Width / GlobalPanelState.DownsampleFactor),
        .Height =
            static_cast<uint32_t>(m_Height / GlobalPanelState.DownsampleFactor),
        .Buffer = m_ImageData.data()};

    PT::Renderer::SaveCapture(captureSpec, GlobalPanelState.ExportFilepath);

    // todo: might want to show a toast after
    // https://github.com/TyomaVader/ImGuiNotify too fancy for now tho

    GlobalEventFlags.ExportToImage = false;
    GlobalPanelState.ExportFilepath = "";
  }
}

void ViewportPanel::RenderScene() {
  auto captureSpec = PT::RenderCaptureSpecification{
      .Width =
          static_cast<uint32_t>(m_Width / GlobalPanelState.DownsampleFactor),
      .Height =
          static_cast<uint32_t>(m_Height / GlobalPanelState.DownsampleFactor),
      .Buffer = m_ImageData.data()};

  // writes the rendered image data to m_ImageData
  m_Renderer.Capture(captureSpec);

  // updates the opengl texture data
  m_RenderedScene.Resize(m_Width / GlobalPanelState.DownsampleFactor,
                         m_Height / GlobalPanelState.DownsampleFactor);
  m_RenderedScene.SetData(m_ImageData.data());
}

void ViewportPanel::ResizeScene() {
  if (m_PrevWidth * m_PrevHeight == 0) {
    // initial render
    RenderScene();
  }

  // update the image buffer
  m_RenderedScene.Resize(m_Width / GlobalPanelState.DownsampleFactor,
                         m_Height / GlobalPanelState.DownsampleFactor);
  m_RenderedScene.SetData(m_ImageData.data());

  m_PrevWidth = m_Width;
  m_PrevHeight = m_Height;
}

}  // namespace Viewer
