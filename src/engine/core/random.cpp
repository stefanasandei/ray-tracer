//
// Created by Asandei Stefan on 22.07.2024.
//

#include "engine/core/random.hpp"

namespace PT {

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

}  // namespace PT
