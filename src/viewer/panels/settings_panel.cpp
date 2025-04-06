//
// Created by Stefan on 7/21/2024.
//

#include "panels/settings_panel.hpp"

namespace Viewer {

SettingsPanel::SettingsPanel() = default;

SettingsPanel::~SettingsPanel() = default;

void SettingsPanel::Render() {
  ImGui::Begin("Settings");

  {
    ImGui::SeparatorText("Scene Settings");

    if(ImGui::Button("Render")) {
      GlobalEventFlags.RenderNow = true;
    }

    ImGui::Text("");
    ImGui::Text("Camera:");

    ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
    if(ImGui::BeginTable("Layout", 2, flags)) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted("Pixel Samples");
      ImGui::SameLine();

      ImGui::TableNextColumn();
      ImGui::SetNextItemWidth(-FLT_MIN); // full column width
      ImGui::InputInt("##samples", reinterpret_cast<int*>(GlobalPanelState.SamplesPerPixel));

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted("Downsample Factor");
      ImGui::SameLine();

      ImGui::TableNextColumn();
      ImGui::SetNextItemWidth(-FLT_MIN); // full column width
      ImGui::InputInt("##downsample_factor", &GlobalPanelState.DownsampleFactor);

      ImGui::EndTable();
    }
  }

  ImGui::Text("");
  if(GlobalPanelState.ActivePrimitiveIdx != -1) {
    auto selectedPrimitive = GlobalPanelState.Scene.GetPrimitives()[GlobalPanelState.ActivePrimitiveIdx-1];

    ImGui::SeparatorText("Entity Settings");

    ImGui::Text("Name: %s", selectedPrimitive.GetTag().c_str());
    ImGui::Text("Selected ID: %d", GlobalPanelState.ActivePrimitiveIdx);
    ImGui::Text("");

    // 1. color picker
    auto material = selectedPrimitive.GetMaterial();
    if (auto lambert = std::dynamic_pointer_cast<PT::Lambertian>(material)) {
      ImVec4 albedo {lambert->GetAlbedo().x, lambert->GetAlbedo().y, lambert->GetAlbedo().z, 1.0f};

      ImGui::DragFloat3("Albedo", &albedo.x, 0.01f, 0.0f, 1.0f);
      ImGui::SameLine();
      ImGui::ColorButton("Albedo", albedo);

      // update the entity
      lambert->SetAlbedo(glm::vec3(albedo.x, albedo.y, albedo.z));
    } else if (auto metal = std::dynamic_pointer_cast<PT::Metal>(material)) {
      ImVec4 albedo {metal->GetAlbedo().x, metal->GetAlbedo().y, metal->GetAlbedo().z, 1.0f};
      float fuzz = metal->GetFuzz();

      ImGui::DragFloat3("Albedo", &albedo.x, 0.01f, 0.0f, 1.0f);
      ImGui::SameLine();
      ImGui::ColorButton("Albedo", albedo);

      ImGui::SliderFloat("Fuzz", &fuzz, 0.0f, 1.0f);

      // update the entity
      metal->SetAlbedo(glm::vec3(albedo.x, albedo.y, albedo.z));
      metal->SetFuzz(fuzz);
    }
  }

  ImGui::End();
}

}  // namespace Viewer
