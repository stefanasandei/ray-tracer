//
// Created by Asandei Stefan on 21.07.2024.
//

#pragma once

#include <glm/glm.hpp>

#include "engine/camera.hpp"
#include "engine/core/base.hpp"
#include "engine/scene.hpp"

namespace PT {

// ------ Helper structures ------

struct RenderCaptureSpecification {
  uint32_t Width, Height;
  uint32_t* Buffer;
};

enum RendererAPI { CPU, VULKAN };

// ------ Main Renderer class ------

class Renderer {
 public:
  Renderer();
  Renderer(const RendererAPI& api);
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
  [[nodiscard]] glm::vec3 PerPixel(uint32_t x, uint32_t y) const noexcept;
  [[nodiscard]] glm::vec3 TraceRay(const Ray& ray,
                                   uint32_t depth = 10) const noexcept;

 private:
  RendererAPI m_API;
  Camera m_Camera;
  Scene m_ScenePrimitive;
};

// ------ Utility function ------

static uint32_t ConvertToRGBA(const glm::vec4& color);

}  // namespace PT
