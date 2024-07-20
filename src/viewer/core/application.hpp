//
// Created by Stefan on 7/20/2024.
//

#pragma once

#include "core/base.hpp"
#include "platform/window.hpp"

namespace Viewer {

class Application {
 public:
  Application();
  ~Application();

  [[nodiscard]] int32_t Run() noexcept;

 private:
  std::shared_ptr<Window> m_Window;
};

}  // namespace Viewer
