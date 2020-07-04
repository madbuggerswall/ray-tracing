#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream>

class Vector3 {
 private:
  double comps[3];  // Scalar components of the vector.

 public:
  Vector3() : comps{0, 0, 0} {}
  Vector3(double x, double y, double z) : comps{x, y, z} {}

  double getX() const {
    return comps[0];
  }
  double getY() const {
    return comps[1];
  }
  double getZ() const {
    return comps[2];
  }

  double magnitude() const {
    return std::sqrt(magnitudeSquared());
  }
  double magnitudeSquared() const {
    return comps[0] * comps[0] + comps[1] * comps[1] + comps[2] * comps[2];
  }

  void normalize() {
    *this / magnitude();
  }
  Vector3 normalized() {
    return Vector3(*this / magnitude());
  }

  Vector3 reflect(const Vector3& normal) {
    return *this - 2 * dot(*this, normal) * normal;
  }
  Vector3 refract(const Vector3& normal, double refractiveRatio) {
    auto cosTheta = dot(-*this, normal);
    Vector3 outParallel = refractiveRatio * (*this + cosTheta * normal);
    Vector3 outPerp = -std::sqrt(1.0 - outParallel.magnitudeSquared()) * normal;
    return outParallel + outPerp;
  }

  inline Vector3 operator-() const {
    return Vector3(-comps[0], -comps[1], -comps[2]);
  }
  inline double operator[](int i) const {
    return comps[i];
  }
  inline double& operator[](int i) {
    return comps[i];
  }

  inline Vector3& operator+=(const Vector3& rhs) {
    comps[0] += rhs.comps[0];
    comps[1] += rhs.comps[1];
    comps[2] += rhs.comps[2];
    return *this;
  }
  inline Vector3& operator-=(const Vector3& rhs) {
    comps[0] -= rhs.comps[0];
    comps[1] -= rhs.comps[1];
    comps[2] -= rhs.comps[2];
    return *this;
  }
  inline Vector3& operator*=(const double value) {
    comps[0] *= value;
    comps[1] *= value;
    comps[2] *= value;
    return *this;
  }
  inline Vector3& operator/=(const double value) {
    return *this *= 1 / value;
  }

  inline Vector3 operator+(const Vector3& rhs) const {
    return Vector3(comps[0] + rhs.comps[0],
                   comps[1] + rhs.comps[1],
                   comps[2] + rhs.comps[2]);
  }
  inline Vector3 operator-(const Vector3& rhs) const {
    return Vector3(comps[0] - rhs.comps[0],
                   comps[1] - rhs.comps[1],
                   comps[2] - rhs.comps[2]);
  }
  inline Vector3 operator*(const Vector3& rhs) const {
    return Vector3(comps[0] * rhs.comps[0],
                   comps[1] * rhs.comps[1],
                   comps[2] * rhs.comps[2]);
  }
  inline Vector3 operator*(double scalar) const {
    return Vector3(comps[0] * scalar, comps[1] * scalar, comps[2] * scalar);
  }
  inline Vector3 operator/(double scalar) const {
    return *this * (1 / scalar);
  }

  // Friend functions are implicitly inline.
  friend Vector3 operator*(double scalar, const Vector3& rhs);
  friend std::ostream& operator<<(std::ostream& out, const Vector3& v);
  friend double dot(const Vector3& lhs, const Vector3& rhs);
  friend Vector3 cross(const Vector3& lhs, const Vector3& rhs);
};

// Definitions of friend functions.
Vector3 operator*(double scalar, const Vector3& rhs) {
  return rhs * scalar;
}
std::ostream& operator<<(std::ostream& out, const Vector3& v) {
  return out << v.comps[0] << ' ' << v.comps[1] << ' ' << v.comps[2];
}
double dot(const Vector3& lhs, const Vector3& rhs) {
  return lhs.comps[0] * rhs.comps[0] + lhs.comps[1] * rhs.comps[1] +
         lhs.comps[2] * rhs.comps[2];
}
Vector3 cross(const Vector3& lhs, const Vector3& rhs) {
  return Vector3(lhs.comps[1] * rhs.comps[2] - lhs.comps[2] * rhs.comps[1],
                 lhs.comps[2] * rhs.comps[0] - lhs.comps[0] * rhs.comps[2],
                 lhs.comps[0] * rhs.comps[1] - lhs.comps[1] * rhs.comps[0]);
}

using Point3 = Vector3;
using Color = Vector3;

#endif