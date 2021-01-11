#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

#include "../Math/Point3.hpp"
#include "../Math/Vector3.hpp"
#include "Math.hpp"

namespace Random {
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.0, 1.0);

  inline float fraction() { return distribution(generator); }
  inline float range(float min, float max) { return min + (max - min) * fraction(); }
	
	// Returns an integer between [min, max).
  inline int rangeInt(int min, int max) { return std::floor(range(min, max)); }

  inline Color color() { return Color(fraction(), fraction(), fraction()); }
  inline Color colorRange(float min, float max) { return Color(range(min, max), range(min, max), range(min, max)); }

  inline Vector3 vector() { return Vector3(fraction(), fraction(), fraction()); }
  inline Vector3 vectorRange(float min, float max) {
    return Vector3(range(min, max), range(min, max), range(min, max));
  }

  inline Point3 pointRange(float min, float max) { return Point3(range(min, max), range(min, max), range(min, max)); }

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

  inline Vector3 cosineDirection() {
    auto r1 = fraction();
    auto r2 = fraction();
    auto phi = 2 * Math::pi * r1;

    auto x = std::cos(phi) * std::sqrt(r2);
    auto y = std::sin(phi) * std::sqrt(r2);
    auto z = std::sqrt(1 - r2);

    return Vector3(x, y, z);
  }

}

#endif