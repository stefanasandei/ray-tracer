//
// Created by Stefan on 7/21/2024.
//

#include <vector>

#include "core/panel.hpp"
#include "gfx/image.hpp"

namespace Viewer {

class ViewportPanel : public Panel {
 public:
  ViewportPanel();
  ~ViewportPanel() = default;

  void Render() override;

 private:
  void RenderScene();
  void ResizeScene();
  void HandlePanelEvents();

 private:
  PT::Renderer m_Renderer;

  int32_t m_Width = 0, m_Height = 0;
  int32_t m_PrevWidth = 0, m_PrevHeight = 0;

  std::vector<uint32_t> m_ImageData;
  Image m_RenderedScene;
};

}  // namespace Viewer