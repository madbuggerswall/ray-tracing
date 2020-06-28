#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "Ray.hpp"
#include "Vector3.hpp"

namespace Util {
  const double infinity = std::numeric_limits<double>::infinity();

  inline double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }

  inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
  }
}

#endif