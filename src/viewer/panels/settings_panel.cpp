//
// Created by Stefan on 7/21/2024.
//

#include "panels/settings_panel.hpp"

namespace Viewer {

SettingsPanel::SettingsPanel() = default;

SettingsPanel::~SettingsPanel() = default;

void SettingsPanel::Render() {
  ImGui::Begin("Settings");

  ImGui::SeparatorText("Scene Settings");

  if(ImGui::Button("Render")) {
    GlobalEventFlags.RenderNow = true;
  }

  ImGui::Text("");
  if(GlobalPanelState.ActivePrimitiveIdx != -1) {
    auto selectedPrimitive = GlobalPanelState.Scene.GetPrimitives()[GlobalPanelState.ActivePrimitiveIdx-1];

    ImGui::SeparatorText("Entity Settings");

    ImGui::Text("name: %s", selectedPrimitive.GetTag().c_str());
    ImGui::Text("selected id: %d", GlobalPanelState.ActivePrimitiveIdx);
  }

  ImGui::End();
}

}  // namespace Viewer
