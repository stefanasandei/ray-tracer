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
