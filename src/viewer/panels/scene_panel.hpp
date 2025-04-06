//
// Created by Stefan on 7/21/2024.
//

#include "core/panel.hpp"

namespace Viewer {

class ScenePanel : public Panel {
 public:
  ScenePanel();
  ~ScenePanel();

  void Render() override;

 private:
  static void LoadDefaultScene();

  void RenderNewPrimitiveForm();
  void RenderSceneHierarchy();
};

}  // namespace Viewer