//
// Created by Stefan on 4/8/2025.
//

#pragma once

#include "engine/backend/backend.h"
#include "engine/camera.hpp"
#include "engine/core/base.hpp"
#include "engine/scene.hpp"

#include <glm/glm.hpp>

namespace PT {

class CPUBackend : public virtual Backend {
 public:
  CPUBackend();
  ~CPUBackend();

  void UpdateRenderPayload(const RenderPayload& payload) override;
  void RenderToBuffer(const RenderCaptureSpecification& spec) override;

 private:
  [[nodiscard]] glm::vec3 PerPixel(uint32_t x, uint32_t y) const noexcept;
  [[nodiscard]] glm::vec3 TraceRay(const Ray& ray,
                                   uint32_t depth = 10) const noexcept;

 private:
  const Camera* m_Camera;
  const Scene* m_ScenePrimitive;
};

}
