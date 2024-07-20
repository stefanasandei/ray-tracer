//
// Created by Stefan on 7/20/2024.
//

#include "application.hpp"

#include <Extensions/NRIDeviceCreation.h>

namespace Viewer {

Application::Application() {
  m_Window = std::make_shared<Window>(WindowSpecification{
      .Width = 1600, .Height = 900, .Title = "Path Tracer Viewer"});

  // nri test usage
  uint32_t adaptersNum = 0;
  nri::Result result = nri::nriEnumerateAdapters(nullptr, adaptersNum);
  if (result != nri::Result::SUCCESS) std::exit(1);

  auto* adapterDesc =
      (nri::AdapterDesc*)malloc(adaptersNum * sizeof(nri::AdapterDesc));
  result = nri::nriEnumerateAdapters(adapterDesc, adaptersNum);
  if (result != nri::Result::SUCCESS) std::exit(1);

  printf("Using GPU %s\n", adapterDesc[0].description);
  free(adapterDesc);
}

Application::~Application() = default;

int32_t Application::Run() noexcept {
  while (!m_Window->ShouldClose()) {
    m_Window->Update();
  }

  return 0;
}

}  // namespace Viewer
