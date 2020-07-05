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

  inline double degreesToRadians(double degrees) {
    return degrees * pi / 180;
  }
}

namespace Random {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());

  inline double range(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
    // return min + (max - min) * fraction();
  }

  inline double fraction() {
    return range(0.0, 1.0);
    // return rand() / (RAND_MAX + 1.0);
  }

  inline Vector3 vector3() {
    return Vector3(fraction(), fraction(), fraction);
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
      auto vector = Random::vectorRange(-1, 1);
      if (vector.magnitudeSquared() >= 1) continue;
      return vector;
    }
  }

  Vector3 vectorInUnitDisk() {
    while (true) {
      auto vector = Vector3(range(-1, 1), range(-1, 1), 0);
      if (vector.magnitudeSquared() >= 1) continue;
      return vector;
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