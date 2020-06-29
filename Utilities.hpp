#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "Ray.hpp"
#include "Vector3.hpp"

namespace Math {
  const double infinity = std::numeric_limits<double>::infinity();
  const double pi = 2 * std::acos(0.0);

  inline double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }
}

namespace Random {
  inline double range(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
  }

  inline double fraction() {
    return range(0.0, 1.0);
  }

  inline Vector3 vectorRange(double min, double max) {
    return Vector3(range(min, max), range(min, max), range(min, max));
  }

  Vector3 unitVector() {
    auto angle = range(0, 2 * Math::pi);
    auto z = range(-1, 1);
    auto r = std::sqrt(1 - z * z);
    return Vector3(r * std::cos(angle), r * std::sin(angle), z);
  }

  Vector3 vectorInUnitSphere() {
    while (true) {
      auto p = Random::vectorRange(-1, 1);
      if (p.magnitudeSquared() >= 1) continue;
      return p;
    }
  }

  Vector3 vectorInHemiSphere(const Vector3& normal) {
    Vector3 inUnitSphere = vectorInUnitSphere();
    if (dot(inUnitSphere, normal) > 0.0)
      return inUnitSphere;
    else
      return -inUnitSphere;
  }
}

#endif