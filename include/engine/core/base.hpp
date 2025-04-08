//
// Created by Stefan on 7/20/2024.
//

#pragma once

#include "engine/core/random.hpp"
#include "engine/core/std.hpp"
#include "engine/core/timer.hpp"

#ifdef _DEBUG

#if defined(_WIN32)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__linux__)
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#define ASSERT(x) \
  if (!x) DEBUG_BREAK()

#else

#define DEBUG_BREAK()
#define ASSERT(x)

#endif

namespace PT {

static inline uint32_t ConvertToRGBA(const glm::vec4& color) {
  auto r = (uint8_t)(color.r * 255.0f);
  auto g = (uint8_t)(color.g * 255.0f);
  auto b = (uint8_t)(color.b * 255.0f);
  auto a = (uint8_t)(color.a * 255.0f);

  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

}
