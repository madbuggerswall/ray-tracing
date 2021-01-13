#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
namespace Math {
  const double infinity = std::numeric_limits<double>::infinity();
  const double pi = 2 * std::acos(0.0);
  const double shadowEpsilon = 0.0001f;

  inline double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }

  inline double degreesToRadians(double degrees) { return degrees * pi / 180; }
  inline double radiansToDegrees(double radians) { return (180 / pi) * radians; }
}

// compensated summation
struct KahanAdder {
  double sum, carry, y;
  KahanAdder(const double b = 0.0) {
    sum = b;
    carry = 0.0;
    y = 0.0;
  }
  inline void add(const double b) {
    y = b - carry;
    const double t = sum + y;
    carry = (t - sum) - y;
    sum = t;
  }
};
#endif