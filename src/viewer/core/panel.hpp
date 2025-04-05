//
// Created by Stefan on 7/21/2024.
//

#pragma once

#include <imgui.h>

#include "core/base.hpp"

namespace Viewer {

// used for panels to send messages to each other
enum class EventFlag {
  RenderNow,
};

class GlobalEventFlags {
 public:
  static void Set(EventFlag flag, bool value = true) {
    flags[flag] = value;
  }

  static bool Get(EventFlag flag) {
    return flags[flag];
  }

  static void Clear(EventFlag flag) {
    flags[flag] = false;
  }

  static void ClearAll() {
    for (auto& [flag, _] : flags) {
      flags[flag] = false;
    }
  }

 private:
  static inline std::unordered_map<EventFlag, bool> flags;
};

class Panel {
 public:
  Panel() = default;
  ~Panel() = default;

  virtual void Render() {}
};

}  // namespace Viewer