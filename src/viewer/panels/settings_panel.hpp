//
// Created by Stefan on 7/21/2024.
//

#pragma once

#include <imgui.h>

#include "core/panel.hpp"
#include "engine/core/timer.hpp"

namespace Viewer {

enum class RenderMode {
  RealTime,
  Offline
};

class SettingsPanel : public Panel {
 public:
  SettingsPanel();
  ~SettingsPanel();

  void Render() override;

 private:
  static void RenderCameraSettings();

  static void RenderEntityMaterialSettings(PT::Primitive& primitive);
  static void RenderEntityTransformSettings(PT::Primitive& primitive);

 private:
  PT::Timer m_Timer;
  RenderMode m_RenderMode;
};

}  // namespace Viewer
