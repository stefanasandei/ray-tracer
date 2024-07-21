//
// Created by Stefan on 7/21/2024.
//

#include "core/panel.hpp"

namespace Viewer {

class ViewportPanel : public Panel {
 public:
  ViewportPanel();
  ~ViewportPanel();

  void Render() override;
};

}  // namespace Viewer