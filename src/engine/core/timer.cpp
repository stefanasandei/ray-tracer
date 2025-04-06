//
// Created by Stefan on 7/23/2024.
//

#include "engine/core/timer.hpp"

namespace PT {

Timer::Timer() = default;

Timer::~Timer() = default;

void Timer::Start() {
  m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

double Timer::End(std::string_view opName, bool output) {
  if(m_StartTimepoint == TimePoint {}) return m_LastResult;

  auto endTimepoint = std::chrono::high_resolution_clock::now();

  auto start =
      std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint)
          .time_since_epoch()
          .count();
  auto end =
      std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint)
          .time_since_epoch()
          .count();

  auto duration = end - start;
  double ms = static_cast<double>(duration) * 0.001;

  m_LastResult = ms;

  if(output)
    std::cout << opName << " took " << ms << " ms\n";

  // reset
  m_StartTimepoint = TimePoint{};

  return ms;
}

}  // namespace PT
