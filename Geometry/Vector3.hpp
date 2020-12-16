#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream>

template <typename T>
class Normal3;
template <typename T>
class Point3;

template <typename T>
class Vector3 {
 public:
  T x, y, z;  // Scalar components of the vector.
  Vector3() : x(0), y(0), z(0) {}
  Vector3(T x, T y, T z) : x(x), y(y), z(z) { assert(!hasNaNs()); }
// Copy constructor
  Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

  // Move constructor
  Vector3(Vector3&& other) :
      x(std::exchange(other.x, 0)),
      y(std::exchange(other.y, 0)),
      z(std::exchange(other.z, 0)) {}

  // Copy assignment
  Vector3& operator=(const Vector3& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  // Move assignment
  Vector3& operator=(Vector3&& other) {
    x = std::move(other.x);
    y = std::move(other.y);
    z = std::move(other.z);
    return *this;
  }
  // Avoid implicitly constructing from a Point.

  explicit Vector3(const Point3<T>& point);
  explicit Vector3(const Normal3<T>& normal);

  // For debugging
  bool hasNaNs() { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

  // Member functions are implicitly inline.
  T magnitude() const { return std::sqrt(magnitudeSquared()); }
  T magnitudeSquared() const { return x * x + y * y + z * z; }

  void normalize() { *this / magnitude(); }
  Vector3 normalized() const { return Vector3(*this / magnitude()); }

  Vector3 reflect(const Vector3& normal) { return *this - 2 * dot(*this, normal) * normal; }
  Vector3 refract(const Vector3& normal, float refractiveRatio) {
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
  T& operator[](int i) {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  // Arithmetic assigment operators
  Vector3& operator+=(const Vector3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Vector3& operator-=(const Vector3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  Vector3& operator*=(const float value) {
    x *= value;
    y *= value;
    z *= value;
    return *this;
  }
  Vector3& operator*=(const Vector3& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }
  Vector3& operator/=(const float scalar) {
    assert(!hasNaNs());
    return *this *= (1.f / scalar);
  }

  // Arithmetic operators
  Vector3 operator-() const { return Vector3(-x, -y, -z); }
  Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
  Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
  Vector3 operator*(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
  Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
  Vector3 operator/(float scalar) const {
    assert(scalar != 0);
    float fraction = 1.f / scalar;
    return Vector3(x * fraction, y * fraction, z * fraction);
  }

  T minComponent() const { return std::min(x, std::min(y, z)); }
  T maxComponent() const { return std::max(x, std::max(y, z)); }

  // Returns the index of the largest component.
  int maxDimension() { return (x > y) ? ((x > z) ? 0 : 2) : ((y > z) ? 1 : 2); }

  // Friend functions are implicitly inline.
  friend Vector3 operator*(float scalar, const Vector3& rhs) { return rhs * scalar; }
  friend std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
  }
  friend T dot(const Vector3& lhs, const Vector3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
  friend T absDot(const Vector3& lhs, const Vector3& rhs) { return std::abs(dot(lhs, rhs)); }
  friend Vector3 abs(const Vector3& v) { return Vector3(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }
  friend Vector3 cross(const Vector3& lhs, const Vector3& rhs) {
    return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
  }
  friend Vector3 min(const Vector3& lhs, const Vector3& rhs) {
    return Vector3<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
  }
  friend Vector3 max(const Vector3& lhs, const Vector3& rhs) {
    return Vector3<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
  }
  friend void createLocalCoordinateSystem(const Vector3& v1, Vector3* v2, Vector3* v3) {
    // This function assumes v1 is normalized.
    if (std::abs(v1.x) > std::abs(v1.y))
      *v2 = Vector3(-v1.z, 0, v1.x).normalized();
    else
      *v2 = Vector3(0, v1.z, -v1.y).normalized();
    *v3 = Cross(v1, *v2);
  }
};

using Vector3F = Vector3<float>;
using Vector3Int = Vector3<int>;

using Color = Vector3<float>;

// Member functions defined in the class body are implicitly inline.

struct UV {
  float u;
  float v;
  UV() : u(0), v(0) {}
  UV(float u, float v) : u(u), v(v) {}
};

#endif