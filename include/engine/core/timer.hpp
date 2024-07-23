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
  void End(std::string_view opName = "X");

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};

}  // namespace PT
