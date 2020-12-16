#ifndef POINT2_HPP
#define POINT2_HPP

#include <cmath>
#include <iostream>

#include "Vector2.hpp"

template <typename T>
class Point2 {
 public:
  T x, y;
  Point2() : x(0), y(0) {}
  Point2(T x, T y) : x(x), y(y) {}

  // Copy constructor
  Point2(const Point2& other) : x(other.x), y(other.y) {}

  // Move constructor
  Point2(Point2&& other) : x(std::exchange(other.x, 0)), y(std::exchange(other.y, 0)) {}

  // Copy assignment
  Point2& operator=(const Point2& other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  // Move assignment
  Point2& operator=(Point2&& other) {
    x = std::move(other.x);
    y = std::move(other.y);
    return *this;
  }

  explicit Point2(const Point3<T>& point) : x(point.x), y(point.y) {}

  // Constructor. Convert Point<T> to Point<U>. Never implicitly.
  template <typename U>
  explicit Point2(const Point2<U>& point) : x((T) point.x), y((T) point.y) {}

  // Cast operator. Convert Point<T> to Vector<U>. Never implicitly.
  template <typename U>
  explicit operator Vector2<U>() const {
    return Vector2<U>(x, y);
  }
  template <typename U>
  explicit Point2(const Vector2<U>& point) : x((T) point.x), y((T) point.y) {}
  // For debugging
  bool HasNaNs() const { return std::isnan(x) || std::isnan(y); }

  // Member access operators
  T operator[](int i) const {
    if (i == 0) return x;
    return y;
  }
  T& operator[](int i) {
    if (i == 0) return x;
    return y;
  }

  // Arithmetic assignment operators
  Point2& operator+=(const Vector2<T>& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  Point2& operator-=(const Vector2<T>& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  Point2& operator+=(const Point2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  Point2& operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  Point2& operator/=(T scalar) {
    float fraction = 1.f / scalar;
    x *= fraction;
    y *= fraction;
    return *this;
  }

  // Arithmetic operators
  Point2 operator-() const { return Point2(-x, -y); }
  Point2 operator+(const Vector2<T>& rhs) const { return Point2(x + rhs.x, y + rhs.y); }
  Point2 operator-(const Vector2<T>& rhs) const { return Point2(x - rhs.x, y - rhs.y); }
  Point2 operator+(const Point2& rhs) const { return Point2(x + rhs.x, y + rhs.y); }
  Point2 operator*(T scalar) const { return Point2(scalar * x, scalar * y); }
  Point2 operator/(T scalar) const {
    float fraction = 1.f / scalar;
    return Point2(fraction * x, fraction * y);
  }
  Vector2<T> operator-(const Point2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }

  // Comparison operators
  bool operator==(const Point2& rhs) const { return x == rhs.x && y == rhs.y; }
  bool operator!=(const Point2& rhs) const { return x != rhs.x || y != rhs.y; }

  friend std::ostream& operator<<(std::ostream& os, const Point2<T>& p) {
    os << "[" << p.x << ", " << p.y << "]";
    return os;
  }
  friend T distance(const Point2& p1, const Point2& p2) { return (p1 - p2).magnitude(); }
  friend T distanceSquared(const Point2& p1, const Point2& p2) { return (p1 - p2).magnitudeSquared(); }
  friend Point2 lerp(float t, const Point2& p1, const Point2& p2) { return (1 - t) * p1 + t * p2; }
  friend Point2 min(const Point2& p1, const Point2& p2) { return Point2(std::min(p1.x, p2.x), std::min(p1.y, p2.y)); }
  friend Point2 max(const Point2& p1, const Point2& p2) { return Point2(std::max(p1.x, p2.x), std::max(p1.y, p2.y)); }
  friend Point2 floor(const Point2& p) { return Point2(std::floor(p.x), std::floor(p.y)); }
  friend Point2 ceil(const Point2& p) { return Point2(std::ceil(p.x), std::ceil(p.y)); }
  friend Point2 abs(const Point2& p) { return Point2(std::abs(p.x), std::abs(p.y)); }
};

using Point2F = Point2<float>;
using Point2Int = Point2<int>;

#endif