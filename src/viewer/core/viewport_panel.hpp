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
  void ResizeScene();

 private:
  PT::Renderer m_Renderer;

  int32_t m_Width = 0, m_Height = 0;
  int32_t m_PrevWidth, m_PrevHeight;

  uint32_t* m_ImageData = nullptr;
  Image m_RenderedScene;
};

}  // namespace Viewer