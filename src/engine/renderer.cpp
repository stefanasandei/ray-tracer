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

  for (uint32_t i = 0; i < spec.Width * spec.Height; i++) {
    spec.Buffer[i] = 0xff00ffff;
  }
}

void Renderer::SaveCapture(const RenderCaptureSpecification& spec,
                           const std::string& filename) const {
  // int stbi_write_png(char const *filename, int w, int h, int comp, const void
  // *data, int stride_in_bytes);

  stbi_write_png(filename.c_str(), spec.Width, spec.Height, 4, spec.Buffer,
                 spec.Width);
}

}  // namespace PT