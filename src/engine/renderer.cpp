//
// Created by Asandei Stefan on 21.07.2024.
//

#include "engine/renderer.hpp"

#include <stb_image_write.h>

namespace PT {

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::AddGeometry(/* Geometry objects */) {}

void Renderer::SetActiveCamera(/* Camera Object */) {}

void Renderer::Capture(RenderCaptureSpecification& spec) const {
  spec.Buffer = new uint32_t[spec.Width * spec.Height];

  for (uint32_t y = 0; y < spec.Height; y++) {
    for (uint32_t x = 0; x < spec.Width; x++) {
      glm::vec4 color =
          glm::vec4(float(x) / (spec.Width - 1) * 2 - 1,
                    float(y) / (spec.Height - 1) * 2 - 1, 0.0f, 1.0f);
      spec.Buffer[x + y * spec.Width] = ConvertToRGBA(color);
    }
  }
}

void Renderer::SaveCapture(const RenderCaptureSpecification& spec,
                           const std::string& filename) const {
  constexpr int numChannels = 4;

  stbi_write_png(filename.c_str(), static_cast<int>(spec.Width),
                 static_cast<int>(spec.Height), numChannels, spec.Buffer,
                 static_cast<int>(spec.Width * sizeof(uint32_t)));
}

static uint32_t ConvertToRGBA(const glm::vec4& color) {
  auto r = (uint8_t)(color.r * 255.0f);
  auto g = (uint8_t)(color.g * 255.0f);
  auto b = (uint8_t)(color.b * 255.0f);
  auto a = (uint8_t)(color.a * 255.0f);

  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

}  // namespace PT
