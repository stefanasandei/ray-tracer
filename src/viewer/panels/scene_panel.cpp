//
// Created by Stefan on 7/21/2024.
//

#include "panels/scene_panel.hpp"

namespace Viewer {

ScenePanel::ScenePanel() {
  LoadDefaultScene();

  GlobalEventFlags.SceneUpdated = true;
}

ScenePanel::~ScenePanel() = default;

void ScenePanel::Render() {
  ImGui::Begin("Scene");

  // to add new scene primitives
  RenderNewPrimitiveForm();

  RenderSceneHierarchy();

  ImGui::End();
}

void ScenePanel::RenderSceneHierarchy() {
  ImGui::Text("List of current primitives:");

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 6.0f));

  uint32_t index = 0;
  for (const auto& primitive : GlobalPanelState.Scene.GetPrimitives()) {
    ++index;

    ImGuiTreeNodeFlags flags = ((GlobalPanelState.ActivePrimitiveIdx == index)
                                    ? ImGuiTreeNodeFlags_Selected
                                    : 0);
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)index, flags, "%s",
                                    primitive.GetTag().c_str());
    if (ImGui::IsItemClicked()) {
      GlobalPanelState.ActivePrimitiveIdx = index;
    }

    if (opened) {
      ImGui::TreePop();
    }
  }

  ImGui::PopStyleVar();
}

void ScenePanel::RenderNewPrimitiveForm() {
  ImGui::Text("Add a new primitive:");

  const char* items[] = {"Sphere"};
  static const char* currentItem = nullptr;

  if (ImGui::BeginCombo("##combo", currentItem)) {
    for (auto& item : items) {
      bool isSelected = (currentItem == item);

      if (ImGui::Selectable(item, isSelected)) currentItem = item;

      if (isSelected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ImGui::SameLine();

  if (currentItem == nullptr) ImGui::BeginDisabled();

  if (ImGui::Button("Create")) {
    PT::Primitive newPrimitive(
        nullptr, std::make_shared<PT::Lambertian>(glm::vec3(1.0f, 1.0f, 1.0f)));
    newPrimitive.SetTag("New Entity");

    if (strcmp(currentItem, "Sphere") == 0) {
      newPrimitive.SetShape(
          std::make_shared<PT::Sphere>(glm::vec3(0.0f, 0.0f, 1.0f), 0.25f));
    }

    GlobalPanelState.Scene.Add(newPrimitive);
    GlobalEventFlags.SceneUpdated = true;
  }

  if (currentItem == nullptr) ImGui::EndDisabled();

  // leave some space between lines
  ImGui::Text(" ");
}

void ScenePanel::LoadDefaultScene() {
  PT::Primitive smolSphere(
      std::make_shared<PT::Sphere>(glm::vec3(0.75f, 0.0f, 1.0f), 0.5f),
      std::make_shared<PT::Lambertian>(glm::vec3(0.8f, 0.0f, 0.9f)));
  smolSphere.SetTag("Smol Sphere");

  PT::Primitive smolSphere2(
      std::make_shared<PT::Sphere>(glm::vec3(-0.75f, 0.0f, 1.0f), 0.5f),
      std::make_shared<PT::Metal>(glm::vec3(0.8f, 0.9f, 0.0f), 0.0f));
  smolSphere2.SetTag("Smol Sphere 2");

  PT::Primitive groundSphere(
      std::make_shared<PT::Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f),
      std::make_shared<PT::Lambertian>(glm::vec3(0.9f, 0.9f, 0.9f)));
  groundSphere.SetTag("Ground Sphere");

  GlobalPanelState.Scene.Add(smolSphere);
  GlobalPanelState.Scene.Add(smolSphere2);
  GlobalPanelState.Scene.Add(groundSphere);
}

}  // namespace Viewer
