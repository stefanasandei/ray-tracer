//
// Created by Asandei Stefan on 21.07.2024.
//

#include <engine/engine.hpp>

int main() {
  PT::Renderer renderer;

  auto captureSpec =
      PT::RenderCaptureSpecification{.Width = 1080, .Height = 720};

  renderer.Capture(captureSpec);

  PT::Renderer::SaveCapture(captureSpec, "sample.png");
  return 0;
}
