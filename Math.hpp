#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
namespace Math {
  const float infinity = std::numeric_limits<float>::infinity();
  const float pi = 2 * std::acos(0.0);
	const float shadowEpsilon = 0.0001f;
	
  inline float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }

  inline float degreesToRadians(float degrees) { return degrees * pi / 180; }
  inline float radiansToDegrees(float radians) { return (180 / pi) * radians; }
}
#endif