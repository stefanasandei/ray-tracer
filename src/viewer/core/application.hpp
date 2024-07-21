//
// Created by Stefan on 7/20/2024.
//

#pragma once

#include "core/base.hpp"
#include "gfx/context.hpp"
#include "panel.hpp"
#include "platform/window.hpp"

namespace Viewer {

class Application {
 public:
  Application();
  ~Application();

  [[nodiscard]] int32_t Run() noexcept;

 private:
  void ShowUI() noexcept;

 private:
  std::shared_ptr<Window> m_Window;
  std::shared_ptr<GfxContext> m_GfxContext;
  std::shared_ptr<UILayer> m_UILayer;

  std::vector<std::shared_ptr<Panel>> m_Panels;
};

}  // namespace Viewer
