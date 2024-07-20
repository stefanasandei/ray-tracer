//
// Created by Asandei Stefan on 20.07.2024.
//

#pragma once

#include <Extensions/NRIDeviceCreation.h>
#include <Extensions/NRIHelper.h>
#include <Extensions/NRIStreamer.h>
#include <Extensions/NRISwapChain.h>

struct NRIInterface : public nri::CoreInterface,
                      public nri::HelperInterface,
                      public nri::StreamerInterface,
                      public nri::SwapChainInterface {};

#define NRI_ABORT_ON_FAILURE(result) \
  if (result != nri::Result::SUCCESS) std::exit(1);
