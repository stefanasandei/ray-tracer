//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include <glm/glm.hpp>

#include "engine/backend/backend.hpp"
#include "engine/camera.hpp"
#include "engine/core/base.hpp"
#include "engine/scene.hpp"

namespace PT {

// ------ Helper structures ------

enum RendererAPI { CPU, VULKAN };

// ------ Main Renderer class ------

class Renderer {
 public:
  explicit Renderer(RendererAPI api = RendererAPI::VULKAN);
  ~Renderer();

  void SetGeometry(const Scene& scene);
  void SetActiveCamera(const Camera& camera);

  [[nodiscard]] const Scene& GetSceneGeometry() const {
    return m_ScenePrimitive;
  }
  [[nodiscard]] Camera& GetCamera() { return m_Camera; }

  void Capture(RenderCaptureSpecification& spec);
  static void SaveCapture(const RenderCaptureSpecification& spec,
                          const std::string& filename);

 private:
  RendererAPI m_API;
  std::shared_ptr<Backend> m_Backend;

  Camera m_Camera;
  Scene m_ScenePrimitive;
};

}  // namespace PT
