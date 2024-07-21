//
// Created by Asandei Stefan on 20.07.2024.
//

#include "gfx/context.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Viewer {

GfxContext::GfxContext(const GfxContextSpecification &spec) {}

GfxContext::~GfxContext() {}

void GfxContext::RenderFrame(uint32_t frameIndex) noexcept {}

}  // namespace Viewer
