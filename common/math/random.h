#pragma once

#include <random>
#include "common/pcg/pcg_random.hpp"

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static pcg64_fast generator;
    return distribution(generator);
}
