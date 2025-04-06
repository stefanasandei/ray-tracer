#include "panels/settings_panel.hpp"

namespace Viewer {

SettingsPanel::SettingsPanel() = default;
SettingsPanel::~SettingsPanel() = default;

namespace {
void CategorySeparator() { ImGui::Text(""); }

constexpr const char* MATERIAL_TYPES[] = { "Lambertian", "Metal" };

std::string GetMaterialType(const std::shared_ptr<PT::Material>& material) {
  if (std::dynamic_pointer_cast<PT::Lambertian>(material)) return "Lambertian";
  if (std::dynamic_pointer_cast<PT::Metal>(material)) return "Metal";
  return "Unknown";
}

std::shared_ptr<PT::Material> CreateMaterial(const std::string& type) {
  if (type == "Lambertian") return std::make_shared<PT::Lambertian>(glm::vec3(1.0f));
  if (type == "Metal")      return std::make_shared<PT::Metal>(glm::vec3(1.0f), 0.0f);
  return nullptr;
}

} // namespace (anonymous)


void SettingsPanel::Render() {
  ImGui::Begin("Settings");

  ImGui::SeparatorText("Scene Settings");

  if (ImGui::Button("Render")) {
    GlobalEventFlags.RenderNow = true;
  }

  CategorySeparator();
  RenderCameraSettings();
  CategorySeparator();

  const uint32_t activeIdx = GlobalPanelState.ActivePrimitiveIdx;
  if (activeIdx > 0) {
    auto& primitive = GlobalPanelState.Scene.GetPrimitives()[activeIdx - 1];

    ImGui::SeparatorText("Entity Settings");

    static char name[256];
    memset(name, 0, sizeof(name));
    strcpy_s(name, sizeof(name), primitive.GetTag().c_str());
    if(ImGui::InputText("Name", name, sizeof(name))) {
      primitive.SetTag(std::string(name));
    }

    ImGui::Text("Selected ID: %d", activeIdx);
    ImGui::Text("");

    RenderEntityMaterialSettings(primitive);
    CategorySeparator();
    RenderEntityTransformSettings(primitive);
  }

  ImGui::End();
}


void SettingsPanel::RenderCameraSettings() {
  ImGui::Text("Camera:");

  if (ImGui::BeginTable("Layout", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
    // sampler per pixel
    ImGui::TableNextRow();
    ImGui::TableNextColumn(); ImGui::AlignTextToFramePadding(); ImGui::Text("Pixel Samples"); ImGui::SameLine();
    ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputInt("##samples", reinterpret_cast<int*>(GlobalPanelState.SamplesPerPixel));

    // downsample factor (to render at a lower resolution)
    ImGui::TableNextRow();
    ImGui::TableNextColumn(); ImGui::AlignTextToFramePadding(); ImGui::Text("Downsample Factor"); ImGui::SameLine();
    ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputInt("##downsample_factor", &GlobalPanelState.DownsampleFactor);

    ImGui::EndTable();
  }
}


void SettingsPanel::RenderEntityMaterialSettings(PT::Primitive& primitive) {
  auto material = primitive.GetMaterial();
  static std::string selectedType = GetMaterialType(material);

  ImGui::Text("Material");

  if (ImGui::BeginCombo("##material_combo", selectedType.c_str())) {
    for (const char* item : MATERIAL_TYPES) {
      bool isSelected = (selectedType == item);
      if (ImGui::Selectable(item, isSelected)) {
        auto newMaterial = CreateMaterial(item);
        if (newMaterial) {
          primitive.SetMaterial(newMaterial);
          selectedType = item;
          GlobalEventFlags.SceneUpdated = true;
        }
      }
      if (isSelected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ImGui::SameLine();
  ImGui::Text("Type");

  // Refresh in case it changed
  material = primitive.GetMaterial();

  if (auto lambert = std::dynamic_pointer_cast<PT::Lambertian>(material)) {
    ImVec4 albedo = { lambert->GetAlbedo().x, lambert->GetAlbedo().y, lambert->GetAlbedo().z, 1.0f };

    ImGui::DragFloat3("Albedo", &albedo.x, 0.01f, 0.0f, 1.0f);
    ImGui::SameLine(); ImGui::ColorButton("Albedo", albedo);

    lambert->SetAlbedo(glm::vec3(albedo.x, albedo.y, albedo.z));

  } else if (auto metal = std::dynamic_pointer_cast<PT::Metal>(material)) {
    ImVec4 albedo = { metal->GetAlbedo().x, metal->GetAlbedo().y, metal->GetAlbedo().z, 1.0f };
    float fuzz = metal->GetFuzz();

    ImGui::DragFloat3("Albedo", &albedo.x, 0.01f, 0.0f, 1.0f);
    ImGui::SameLine(); ImGui::ColorButton("Albedo", albedo);

    ImGui::SliderFloat("Fuzz", &fuzz, 0.0f, 1.0f);

    metal->SetAlbedo(glm::vec3(albedo.x, albedo.y, albedo.z));
    metal->SetFuzz(fuzz);
  }
}


void SettingsPanel::RenderEntityTransformSettings(PT::Primitive& primitive) {
  auto shape = primitive.GetShape();
  ImGui::Text("Transform");

  if (auto sphere = std::dynamic_pointer_cast<PT::Sphere>(shape)) {
    ImVec4 origin = { sphere->GetOrigin().x, sphere->GetOrigin().y, sphere->GetOrigin().z, 1.0f };
    float radius = sphere->GetRadius();

    ImGui::DragFloat3("Origin", &origin.x, 0.005f, -2.0f, 2.0f);
    ImGui::SliderFloat("Radius", &radius, 0.001f, 2.5f);

    sphere->SetOrigin(glm::vec3(origin.x, origin.y, origin.z));
    sphere->SetRadius(radius);
  }
}

} // namespace Viewer
