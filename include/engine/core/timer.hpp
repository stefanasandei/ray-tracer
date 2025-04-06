//
// Created by Stefan on 7/23/2024.
//

#pragma once

#include "engine/core/base.hpp"

namespace PT {

class Timer {
 public:
  Timer();
  ~Timer();

  void Start();
  double End(std::string_view opName = "X", bool output = true);

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
  double m_LastResult = 0.0;
};

using TimePoint = const std::chrono::time_point<std::chrono::steady_clock>;

}  // namespace PT
