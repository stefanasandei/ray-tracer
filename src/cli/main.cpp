//
// Created by Asandei Stefan on 21.07.2024.
//

#include <engine/engine.hpp>

int main() {
  PT::Timer globalTimer;
  PT::Timer timer;

  globalTimer.Start();

  PT::Renderer renderer;

  timer.Start();
  PT::Camera camera({.VerticalFOV = 45.0f,
                     .NearClip = 0.1f,
                     .FarClip = 100.0f,
                     .Width = 1080,
                     .Height = 720});
  timer.End("Camera creation");

  PT::Primitive smolSphere(
      std::make_shared<PT::Sphere>(glm::vec3(0.75f, 0.0f, 1.0f), 0.5f),
      std::make_shared<PT::Lambertian>(glm::vec3(0.8f, 0.0f, 0.9f)));

  PT::Primitive smolSphere2(
      std::make_shared<PT::Sphere>(glm::vec3(-0.75f, 0.0f, 1.0f), 0.5f),
      std::make_shared<PT::Metal>(glm::vec3(0.8f, 0.9f, 0.0f), 0.0f));

  PT::Primitive groundSphere(
      std::make_shared<PT::Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f),
      std::make_shared<PT::Lambertian>(glm::vec3(0.9f, 0.9f, 0.9f)));

  PT::Scene scene;
  scene.Add(smolSphere);
  scene.Add(smolSphere2);
  scene.Add(groundSphere);

  renderer.SetActiveCamera(camera);
  renderer.SetGeometry(scene);

  constexpr auto factor = 1;
  auto captureSpec = PT::RenderCaptureSpecification{.Width = 1280 / factor,
                                                    .Height = 720 / factor};

  timer.Start();
  renderer.Capture(captureSpec);
  timer.End("Rendering");

  PT::Renderer::SaveCapture(captureSpec, "sample.png");
  globalTimer.End("CLI");
  return 0;
}
