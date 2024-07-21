//
// Created by Stefan on 7/21/2024.
//

#include "core/settings_panel.hpp"

namespace Viewer {

SettingsPanel::SettingsPanel() = default;

SettingsPanel::~SettingsPanel() = default;

void SettingsPanel::Render() {
  ImGui::Begin("Settings");
  ImGui::Text("hello world");
  ImGui::End();
}

}  // namespace Viewer
