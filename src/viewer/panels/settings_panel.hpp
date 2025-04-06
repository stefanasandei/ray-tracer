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
};

}  // namespace Viewer