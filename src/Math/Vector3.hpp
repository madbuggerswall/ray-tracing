#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream>

class Normal3;
class Point3;

class Vector3 {
 public:
  double x, y, z;  // Scalar components of the vector.
  Vector3() : x(0), y(0), z(0) {}
  Vector3(double x, double y, double z) : x(x), y(y), z(z) {
    // assert(!hasNaNs());
  }
  // Copy constructor
  Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

  // Move constructor
  Vector3(Vector3&& other) noexcept :
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
  Vector3& operator=(Vector3&& other) noexcept {
    x = std::move(other.x);
    y = std::move(other.y);
    z = std::move(other.z);
    return *this;
  }
  // Avoid implicitly constructing from a Point.

  explicit Vector3(const Point3& point);
  explicit Vector3(const Normal3& normal);

  // For debugging
  bool hasNaNs() { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

  // Member functions are implicitly inline.
  double magnitude() const { return std::sqrt(magnitudeSquared()); }
  double magnitudeSquared() const { return x * x + y * y + z * z; }

  void normalize() { *this /= magnitude(); }
  Vector3 normalized() const { return Vector3(*this / magnitude()); }

  Vector3 reflect(const Vector3& normal) const { return *this - 2 * dot(*this, normal) * normal; }
  Vector3 refract(const Vector3& normal, double refractiveRatio) const {
    const auto cosTheta = dot(-*this, normal);
    const Vector3 outParallel = refractiveRatio * (*this + cosTheta * normal);
    const Vector3 outPerp = -std::sqrt(1.0 - outParallel.magnitudeSquared()) * normal;
    return outParallel + outPerp;
  }

  // Member access operators
  double operator[](int i) const {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }
  double& operator[](int i) {
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
  Vector3& operator*=(const Vector3& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }
  Vector3& operator*=(const double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  Vector3& operator/=(const double scalar) {
    assert(!hasNaNs());
    return *this *= (1.f / scalar);
  }

  // Arithmetic operators
  Vector3 operator-() const { return Vector3(-x, -y, -z); }
  Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
  Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
  Vector3 operator*(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
  Vector3 operator*(double scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
  Vector3 operator/(double scalar) const {
    assert(scalar != 0);
    double fraction = 1.f / scalar;
    return Vector3(x * fraction, y * fraction, z * fraction);
  }

  double minComponent() const { return std::min(x, std::min(y, z)); }
  double maxComponent() const { return std::max(x, std::max(y, z)); }

  // Returns the index of the largest component.
  int maxDimension() { return (x > y) ? ((x > z) ? 0 : 2) : ((y > z) ? 1 : 2); }

  // Friend functions are implicitly inline.
  friend Vector3 operator*(double scalar, const Vector3& rhs);
  friend std::ostream& operator<<(std::ostream& out, const Vector3& vector) {
    return out << vector.x << ' ' << vector.y << ' ' << vector.z;
  }
  friend double dot(const Vector3& lhs, const Vector3& rhs);
  friend double absDot(const Vector3& lhs, const Vector3& rhs) { return std::abs(dot(lhs, rhs)); }
  friend Vector3 abs(const Vector3& vec) { return Vector3(std::abs(vec.x), std::abs(vec.y), std::abs(vec.z)); }
  friend Vector3 cross(const Vector3& lhs, const Vector3& rhs);
  friend Vector3 min(const Vector3& lhs, const Vector3& rhs) {
    return Vector3(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
  }
  friend Vector3 max(const Vector3& lhs, const Vector3& rhs) {
    return Vector3(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
  }
};

// Friend function definitions.
Vector3 operator*(double scalar, const Vector3& rhs) { return rhs * scalar; }
double dot(const Vector3& lhs, const Vector3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
Vector3 cross(const Vector3& lhs, const Vector3& rhs) {
  return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

// Member functions defined in the class body are implicitly inline.

struct UV {
  double u;
  double v;
  UV() : u(0), v(0) {}
  UV(double u, double v) : u(u), v(v) {}
};

class Color {
 public:
  double red;
  double green;
  double blue;

  Color() : red(0), green(0), blue(0) {}
  Color(double red, double green, double blue) : red(red), green(green), blue(blue) { assert(!hasNaNs()); }

  // Copy constructor
  Color(const Color& other) : red(other.red), green(other.green), blue(other.blue) { assert(!hasNaNs()); }

  // Move constructor
  Color(Color&& other) :
      red(std::exchange(other.red, 0)),
      green(std::exchange(other.green, 0)),
      blue(std::exchange(other.blue, 0)) {
    assert(!hasNaNs());
  }

  // Copy assignment
  Color& operator=(const Color& other) {
    red = other.red;
    green = other.green;
    blue = other.blue;
    assert(!hasNaNs());
    return *this;
  }

  // Move assignment
  Color& operator=(Color&& other) noexcept {
    red = std::move(other.red);
    green = std::move(other.green);
    blue = std::move(other.blue);
    assert(!hasNaNs());
    return *this;
  }

  // For debugging
  bool hasNaNs() { return std::isnan(red) || std::isnan(green) || std::isnan(blue); }
  bool hasInfs() { return std::isinf(red) || std::isinf(green) || std::isinf(blue); }

  inline double maxComponent() const { return std::fmax(red, std::fmax(green, blue)); }

  Color operator+(const Color& rhs) const { return Color(red + rhs.red, green + rhs.green, blue + rhs.blue); }
  Color operator-(const Color& rhs) const { return Color(red - rhs.red, green - rhs.green, blue - rhs.blue); }
  Color operator*(const Color& rhs) const { return Color(red * rhs.red, green * rhs.green, blue * rhs.blue); }
  Color operator*(double scalar) const { return Color(red * scalar, green * scalar, blue * scalar); }
  Color operator/(double scalar) const {
    assert(scalar != 0);
    double fraction = 1.f / scalar;
    return Color(red * fraction, green * fraction, blue * fraction);
  }

  Color& operator+=(const Color& rhs) {
    red += rhs.red;
    green += rhs.green;
    blue += rhs.blue;
    return *this;
  }
  Color& operator*=(const Color& rhs) {
    red *= rhs.red;
    green *= rhs.green;
    blue *= rhs.blue;
    return *this;
  }
  Color& operator*=(double scalar) {
    red *= scalar;
    green *= scalar;
    blue *= scalar;
    return *this;
  }
};

#endif