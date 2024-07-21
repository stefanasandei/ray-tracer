//
// Created by Stefan on 7/21/2024.
//

#include "core/panel.hpp"
#include "gfx/image.hpp"

namespace Viewer {

class ViewportPanel : public Panel {
 public:
  ViewportPanel();
  ~ViewportPanel();

  void Render() override;

 private:
  void RenderScene();

 private:
  PT::Renderer m_Renderer;

  int32_t m_Width = 1080, m_Height = 720;
  uint32_t* m_ImageData = nullptr;
  Image m_RenderedScene;
};

}  // namespace Viewer