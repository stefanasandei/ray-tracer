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
  // todo: be able to select one

  ImGui::Text("List of current primitives:");
  uint32_t index = 0;
  for(const auto& primitive: GlobalPanelState.Scene.GetPrimitives()) {
    ++index;

    ImGuiTreeNodeFlags flags = ((GlobalPanelState.ActivePrimitiveIdx == index) ? ImGuiTreeNodeFlags_Selected : 0);
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)index, flags, "Primitive id: %d", index);
    if(ImGui::IsItemClicked()) {
      GlobalPanelState.ActivePrimitiveIdx = index;
    }

    if(opened) {
      ImGui::TreePop();
    }
  }
}

void ScenePanel::RenderNewPrimitiveForm() {
  // todo: functionality

  ImGui::Text("Add a new primitive:");

  const char* items[] = { "Sphere" };
  static const char* current_item = NULL;

  if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
  {
    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
    {
      bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
      if (ImGui::Selectable(items[n], is_selected))
        current_item = items[n];
      if (is_selected)
        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
    }
    ImGui::EndCombo();
  }
  ImGui::SameLine();
  if(ImGui::Button("Create")) {
    GlobalEventFlags.SceneUpdated = true;
  }
  ImGui::Text(" "); // leave some space between lines
}

void ScenePanel::LoadDefaultScene() {
  PT::Primitive smolSphere(
      std::make_shared<PT::Sphere>(glm::vec3(0.75f, 0.0f, 1.0f), 0.5f),
      std::make_shared<PT::Lambertian>(glm::vec3(0.8f, 0.0f, 0.9f)));

  PT::Primitive smolSphere2(
      std::make_shared<PT::Sphere>(glm::vec3(-0.75f, 0.0f, 1.0f), 0.5f),
      std::make_shared<PT::Metal>(glm::vec3(0.8f, 0.9f, 0.0f), 0.0f));

  PT::Primitive groundSphere(
      std::make_shared<PT::Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f),
      std::make_shared<PT::Lambertian>(glm::vec3(0.9f, 0.9f, 0.9f)));

  GlobalPanelState.Scene.Add(smolSphere);
  GlobalPanelState.Scene.Add(smolSphere2);
  GlobalPanelState.Scene.Add(groundSphere);
}

}  // namespace Viewer
