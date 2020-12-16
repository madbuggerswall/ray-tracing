#ifndef POINT3_HPP
#define POINT3_HPP

#include <cmath>
#include <iostream>

#include "Vector3.hpp"

template <typename T>
class Point3 {
 public:
  T x, y, z;
  Point3() : x(0), y(0), z(0) {}
  Point3(T x, T y, T z) : x(x), y(y), z(z) {}

  // For debugging
  bool HasNaNs() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

  // Member access operators
  T operator[](int i) const {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }
  T &operator[](int i) {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  // Arithmetic assignment operators
  Point3 &operator+=(const Vector3<T> &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Point3 &operator-=(const Vector3<T> &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  Point3 &operator+=(const Point3 &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Point3 &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  Point3 &operator/=(T scalar) {
    float fraction = 1.f / scalar;
    x *= fraction;
    y *= fraction;
    z *= fraction;
    return *this;
  }

  // Arithmetic operators
  Point3 operator-() const { return Point3(-x, -y, -z); }
  Point3 operator+(const Vector3<T> &rhs) const { return Point3(x + rhs.x, y + rhs.y, z + rhs.z); }
  Point3 operator-(const Vector3<T> &rhs) const { return Point3(x - rhs.x, y - rhs.y, z - rhs.z); }
  Point3 operator+(const Point3 &rhs) const { return Point3(x + rhs.x, y + rhs.y, z + rhs.z); }
  Point3 operator*(T scalar) const { return Point3(scalar * x, scalar * y, scalar * z); }
  Point3 operator/(T scalar) const {
    float fraction = 1.f / scalar;
    return Point3(fraction * x, fraction * y, fraction * z);
  }
  Vector3<T> operator-(const Point3 &rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }

  // Comparison operators
  bool operator==(const Point3 &rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
  bool operator!=(const Point3 &rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

  friend std::ostream &operator<<(std::ostream &os, const Point3<T> &p) {
    os << "[" << p.x << ", " << p.y << ", " << p.z << "]";
    return os;
  }
  friend T distance(const Point3 &p1, const Point3 &p2) { return (p1 - p2).magnitude(); }
  friend T distanceSquared(const Point3 &p1, const Point3 &p2) { return (p1 - p2).magnitudeSquared(); }
  friend Point3 lerp(float t, const Point3 &p1, const Point3 &p2) { return (1 - t) * p1 + t * p2; }
  friend Point3 min(const Point3 &p1, const Point3 &p2) {
    return Point3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
  }
  friend Point3 max(const Point3 &p1, const Point3 &p2) {
    return Point3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
  }
  friend Point3 floor(const Point3 &p) { return Point3(std::floor(p.x), std::floor(p.y), std::floor(p.z)); }
  friend Point3 ceil(const Point3 &p) { return Point3(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z)); }
  friend Point3 abs(const Point3 &p) { return Point3(std::abs(p.x), std::abs(p.y), std::abs(p.z)); }
  friend Point3 permute(const Point3 &p, int x, int y, int z) { return Point3(p[x], p[y], p[z]); }

  // Constructor. Convert Point<T> to Point<U>. Never implicitly.
  template <typename U>
  explicit Point3(const Point3<U> &p) : x((T) p.x), y((T) p.y), z((T) p.z) {}

  // Cast operator. Convert Point<T> to Vector<U>. Never implicitly.
  template <typename U>
  explicit operator Vector3<U>() const {
    return Vector3<U>(x, y, z);
  }
};

using Point3F = Point3<float>;
using Point3Int = Point3<int>;

#endif