//
// Created by Stefan on 7/21/2024.
//

#include "core/viewport_panel.hpp"

namespace Viewer {

ViewportPanel::ViewportPanel() = default;

ViewportPanel::~ViewportPanel() = default;

void ViewportPanel::Render() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

  ImGui::Begin("Viewport");
  ImGui::Text("hello world");
  ImGui::End();

  ImGui::PopStyleVar();
}

}  // namespace Viewer
