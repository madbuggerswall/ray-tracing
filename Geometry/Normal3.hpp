#ifndef NORMAL3_HPP
#define NORMAL3_HPP

#include <cmath>
#include <iostream>

#include "Vector3.hpp"

template <typename T>
class Normal3 {
 public:
  T x, y, z;
  Normal3() {}
  Normal3(T x, T y, T z) : x(x), y(y), z(z) {}

  explicit Normal3(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z) {}

  bool hasNaNs() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

  T magnitude() const { return std::sqrt(magnitudeSquared()); }
  T magnitudeSquared() const { return x * x + y * y + z * z; }

  void normalize() { *this / magnitude(); }
  Normal3 normalized() const { return Normal3(*this / magnitude()); }

  Normal3 reflect(const Normal3 &normal) { return *this - 2 * dot(*this, normal) * normal; }
  Normal3 refract(const Normal3 &normal, float refractiveRatio) {
    auto cosTheta = dot(-*this, normal);
    Vector3 outParallel = refractiveRatio * (*this + cosTheta * normal);
    Vector3 outPerp = -std::sqrt(1.0 - outParallel.magnitudeSquared()) * normal;
    return outParallel + outPerp;
  }

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
  Normal3 &operator+=(const Normal3 &normal) {
    x += normal.x;
    y += normal.y;
    z += normal.z;
    return *this;
  }
  Normal3 &operator-=(const Normal3 &normal) {
    x -= normal.x;
    y -= normal.y;
    z -= normal.z;
    return *this;
  }
  Normal3 &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  Normal3 &operator/=(T scalar) {
    assert(!hasNaNs());
    return *this *= (1.f / scalar);
  }

  // Arithmetic operators
  Normal3 operator*(T scalar) const { return Normal3(scalar * x, scalar * y, scalar * z); }
  Normal3 operator-() const { return Normal3(-x, -y, -z); }
  Normal3 operator+(const Normal3 &n) const { return Normal3(x + n.x, y + n.y, z + n.z); }
  Normal3 operator-(const Normal3 &n) const { return Normal3(x - n.x, y - n.y, z - n.z); }
  Normal3 operator/(T scalar) const {
    assert(scalar != 0);
    float fraction = 1.f / scalar;
    return Normal3(x * fraction, y * fraction, z * fraction);
  }

  // Comparison operators
  bool operator==(const Normal3<T> &normal) const { return x == normal.x && y == normal.y && z == normal.z; }
  bool operator!=(const Normal3<T> &normal) const { return x != normal.x || y != normal.y || z != normal.z; }

  // Friend functions
  friend T dot(const Normal3 &n, const Vector3<T> &v) { return n.x * v.x + n.y * v.y + n.z * v.z; }
  friend T dot(const Vector3<T> &v, const Normal3 &n) { return v.x * n.x + v.y * n.y + v.z * n.z; }
  friend T dot(const Normal3 &n1, const Normal3 &n2) { return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z; }
  friend T absDot(const Normal3 &n, const Vector3<T> &v) { return std::abs(n.x * v.x + n.y * v.y + n.z * v.z); }
  friend T absDot(const Vector3<T> &v, const Normal3 &n) { return std::abs(v.x * n.x + v.y * n.y + v.z * n.z); }
  friend T absDot(const Normal3 &n1, const Normal3 &n2) { return std::abs(n1.x * n2.x + n1.y * n2.y + n1.z * n2.z); }
  friend Normal3 abs(const Normal3 &n) { return Normal3(std::abs(n.x), std::abs(n.y), std::abs(n.z)); }
  friend Normal3 faceForward(const Normal3 &n, const Vector3<T> &v) { return (dot(n, v) < 0.f) ? -n : n; }
  friend Normal3 faceForward(const Normal3 &n1, const Normal3 &n2) { return (dot(n1, n2) < 0.f) ? -n1 : n1; }
  friend Vector3<T> faceForward(const Vector3<T> &v1, const Vector3<T> &v2) { return (dot(v1, v2) < 0.f) ? -v1 : v1; }
  friend Vector3<T> faceForward(const Vector3<T> &v, const Normal3 &n) { return (dot(v, n) < 0.f) ? -v : v; }
  template <typename U>
  friend Normal3 operator*(U scalar, const Normal3 &n) {
    return Normal3(scalar * n.x, scalar * n.y, scalar * n.z);
  }
  friend std::ostream &operator<<(std::ostream &os, const Normal3 &v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
  }
};

using Normal3F = Normal3<float>;
using Normal3Int = Normal3<int>;

#endif