#ifndef NORMAL3_HPP
#define NORMAL3_HPP

#include <cmath>
#include <iostream>

#include "Vector3.hpp"

class Normal3 {
 public:
  float x, y, z;
  Normal3() {}
  Normal3(float x, float y, float z) : x(x), y(y), z(z) {}

  // Copy constructor
  Normal3(const Normal3& other) : x(other.x), y(other.y), z(other.z) {}

  // Move constructor
  Normal3(Normal3&& other) : x(std::exchange(other.x, 0)), y(std::exchange(other.y, 0)), z(std::exchange(other.z, 0)) {}

  // Copy assignment
  Normal3& operator=(const Normal3& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  // Move assignment
  Normal3& operator=(Normal3&& other) {
    x = std::move(other.x);
    y = std::move(other.y);
    z = std::move(other.z);
    return *this;
  }

  explicit Normal3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

  bool hasNaNs() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

  float magnitude() const { return std::sqrt(magnitudeSquared()); }
  float magnitudeSquared() const { return x * x + y * y + z * z; }

  void normalize() { *this / magnitude(); }
  Normal3 normalized() const { return Normal3(*this / magnitude()); }

  // Member access operators
  float operator[](int i) const {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }
  float& operator[](int i) {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  // Arithmetic assignment operators
  Normal3& operator+=(const Normal3& normal) {
    x += normal.x;
    y += normal.y;
    z += normal.z;
    return *this;
  }
  Normal3& operator-=(const Normal3& normal) {
    x -= normal.x;
    y -= normal.y;
    z -= normal.z;
    return *this;
  }
  Normal3& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  Normal3& operator/=(float scalar) {
    assert(!hasNaNs());
    return *this *= (1.f / scalar);
  }

  // Arithmetic operators
  Normal3 operator*(float scalar) const { return Normal3(scalar * x, scalar * y, scalar * z); }
  Normal3 operator-() const { return Normal3(-x, -y, -z); }
  Normal3 operator+(const Normal3& n) const { return Normal3(x + n.x, y + n.y, z + n.z); }
  Normal3 operator-(const Normal3& n) const { return Normal3(x - n.x, y - n.y, z - n.z); }
  Normal3 operator/(float scalar) const {
    assert(scalar != 0);
    float fraction = 1.f / scalar;
    return Normal3(x * fraction, y * fraction, z * fraction);
  }

  // Comparison operators
  bool operator==(const Normal3& normal) const { return x == normal.x && y == normal.y && z == normal.z; }
  bool operator!=(const Normal3& normal) const { return x != normal.x || y != normal.y || z != normal.z; }

  // Friend functions
  friend float dot(const Normal3& n, const Vector3& v);
  friend float dot(const Vector3& v, const Normal3& n);
  friend float dot(const Normal3& n1, const Normal3& n2);
  friend float absDot(const Normal3& n, const Vector3& v) { return std::abs(n.x * v.x + n.y * v.y + n.z * v.z); }
  friend float absDot(const Vector3& v, const Normal3& n) { return std::abs(v.x * n.x + v.y * n.y + v.z * n.z); }
  friend float absDot(const Normal3& n1, const Normal3& n2) {
    return std::abs(n1.x * n2.x + n1.y * n2.y + n1.z * n2.z);
  }
  friend Normal3 abs(const Normal3& n) { return Normal3(std::abs(n.x), std::abs(n.y), std::abs(n.z)); }
  friend Normal3 faceForward(const Normal3& n, const Vector3& v) { return (dot(n, v) < 0.f) ? -n : n; }
  friend Normal3 faceForward(const Normal3& n1, const Normal3& n2) { return (dot(n1, n2) < 0.f) ? -n1 : n1; }
  friend Vector3 faceForward(const Vector3& v1, const Vector3& v2) { return (dot(v1, v2) < 0.f) ? -v1 : v1; }
  friend Vector3 faceForward(const Vector3& v, const Normal3& n) { return (dot(v, n) < 0.f) ? -v : v; }
  friend Normal3 operator*(float scalar, const Normal3& n);
  friend std::ostream& operator<<(std::ostream& os, const Normal3& v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
  }
};

Normal3 operator*(float scalar, const Normal3& n) { return Normal3(scalar * n.x, scalar * n.y, scalar * n.z); }
float dot(const Normal3& n, const Vector3& v) { return n.x * v.x + n.y * v.y + n.z * v.z; }
float dot(const Vector3& v, const Normal3& n) { return v.x * n.x + v.y * n.y + v.z * n.z; }
float dot(const Normal3& n1, const Normal3& n2) { return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z; }

#endif