//
// Created by Stefan on 7/20/2024.
//

#include "gfx/ui_layer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdlib>

namespace Viewer {

UILayer::UILayer(GLFWwindow* glfwHandle) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(glfwHandle, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}

UILayer::~UILayer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void UILayer::BeginUI() noexcept {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  static bool optFullscreenPersistent = true;
  static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (optFullscreenPersistent) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove;
    windowFlags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }

  if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    windowFlags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("###DockSpace", &m_Open, windowFlags);
  ImGui::PopStyleVar();

  if (optFullscreenPersistent) ImGui::PopStyleVar(2);

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
  }

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Close")) {
        std::exit(0);
      }

      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  ImGui::End();
}

void UILayer::EndUI() noexcept {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

}  // namespace Viewer
