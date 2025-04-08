//
// Created by Stefan on 4/8/2025.
//

#pragma once

#include "engine/core/base.hpp"
#include "engine/core/std.hpp"

namespace PT {

class DeletionQueue {
 public:
  DeletionQueue() = default;
  ~DeletionQueue() = default;

  inline void Push(std::function<void()>&& function) {
    m_Deletors.push_back(function);
  }

  inline void Flush() {
    // delete in the reversed order the objects were created
    for (auto it = m_Deletors.rbegin(); it != m_Deletors.rend(); it++) {
      (*it)();
    }

    m_Deletors.clear();
  }

 private:
  std::deque<std::function<void()>> m_Deletors;
};

}
