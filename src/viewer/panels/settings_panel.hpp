//
// Created by Stefan on 7/21/2024.
//

#include "core/panel.hpp"

namespace Viewer {

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
};

}  // namespace Viewer
