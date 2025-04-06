//
// Created by Stefan on 7/21/2024.
//

#include "panels/settings_panel.hpp"

namespace Viewer {

SettingsPanel::SettingsPanel() = default;

SettingsPanel::~SettingsPanel() = default;

void SettingsPanel::Render() {
  ImGui::Begin("Settings");

  if(ImGui::Button("Render")) {
    GlobalEventFlags.RenderNow = true;
  }

  ImGui::End();
}

}  // namespace Viewer
