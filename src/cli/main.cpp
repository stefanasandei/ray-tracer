//
// Created by Asandei Stefan on 21.07.2024.
//

#include <engine/engine.hpp>

int main() {
  PT::Renderer renderer;

  auto captureSpec = PT::RenderCaptureSpecification{
      .Width = 1080, .Height = 720, .Buffer = nullptr};

  renderer.Capture(captureSpec);
  renderer.SaveCapture(captureSpec, "sample.png");

  return 0;
}
