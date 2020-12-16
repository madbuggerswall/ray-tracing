#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <iostream>

template <typename T>
class Point2;
template <typename T>
class Point3;

template <typename T>
class Vector2 {
 public:
  T x, y;
  Vector2() : x(0), y(0) {}
  Vector2(T x, T y) : x(x), y(y) { assert(!hasNaNs()); }

  // Copy constructor
  Vector2(const Vector2& other) : x(other.x), y(other.y) {}

  // Move constructor
  Vector2(Vector2&& other) : x(std::exchange(other.x, 0)), y(std::exchange(other.y, 0)) {}

  // Copy assignment
  Vector2& operator=(const Vector2& other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  // Move assignment
  Vector2& operator=(Vector2&& other) {
    x = std::move(other.x);
    y = std::move(other.y);
    return *this;
  }

  explicit Vector2(const Point2<T>& point);
  explicit Vector2(const Point3<T>& point);

  bool hasNaNs() { return std::isnan(x) || std::isnan(y); }

  T magnitude() const { return std::sqrt(magnitudeSquared()); }
  T magnitudeSquared() const { return x * x + y * y; }

  void normalize() { *this / magnitude(); }
  Vector2 normalized() const { return Vector2(*this / magnitude()); }

  Vector2 reflect(const Vector2& normal) { return *this - 2 * dot(*this, normal) * normal; }
  Vector2 refract(const Vector2& normal, float refractiveRatio) {
    auto cosTheta = dot(-*this, normal);
    Vector2 outParallel = refractiveRatio * (*this + cosTheta * normal);
    Vector2 outPerp = -std::sqrt(1.0 - outParallel.magnitudeSquared()) * normal;
    return outParallel + outPerp;
  }

  // Member functions are implicitly inline.
  Vector2 operator-() const { return Vector2(-x, -y); }
  T operator[](int i) const {
    if (i == 0)
      return x;
    else if (i == 1)
      return y;
  }
  T& operator[](int i) {
    if (i == 0)
      return x;
    else if (i == 1)
      return y;
  }

  // Arithmetic assignment operators
  Vector2& operator+=(const Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  Vector2& operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  Vector2& operator*=(const float value) {
    x *= value;
    y *= value;
    return *this;
  }
  Vector2& operator*=(const Vector2& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  }
  Vector2& operator/=(const float value) {
    assert(!hasNaNs());
    return *this *= 1 / value;
  }

  // Aritmetic operators
  Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
  Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
  Vector2 operator*(const Vector2& rhs) const { return Vector2(x * rhs.x, y * rhs.y); }
  Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
  Vector2 operator/(float scalar) const {
    assert(scalar != 0);
    float fraction = 1 / scalar;
    return Vector2(x * fraction, y * fraction);
  }

  // Friend functions are implicitly inline.
  friend Vector2 operator*(float scalar, const Vector2& rhs) { return rhs * scalar; }
  friend std::ostream& operator<<(std::ostream& out, const Vector2& v) { return out << v.x << ' ' << v.y; }
  friend T dot(const Vector2& lhs, const Vector2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
  friend T absDot(const Vector2& lhs, const Vector2& rhs) { return std::abs(dot(lhs, rhs)); }
};

using Vector2F = Vector2<float>;
using Vector2Int = Vector2<int>;

#endif