#include <cmath>
#include <iostream>

class Vector3 {
 private:
  double comps[3];  // Scalar components of the vector.

 public:
  Vector3() : comps{0, 0, 0} {}
  Vector3(double x, double y, double z) : comps{x, y, z} {}

  double getX() const { return comps[0]; }
  double getY() const { return comps[1]; }
  double getZ() const { return comps[2]; }

  double magnitude() const { return std::sqrt(magnitudeSquared()); }
  double magnitudeSquared() const {
    return comps[0] * comps[0] + comps[1] * comps[1] + comps[2] * comps[2];
  }

  void normalize() { *this / magnitude(); }
  Vector3 normalized() { return Vector3(*this / magnitude()); }

  Vector3 operator-() const { return Vector3(-comps[0], -comps[1], -comps[2]); }
  double operator[](int i) const { return comps[i]; }
  double& operator[](int i) { return comps[i]; }

  inline Vector3 operator+(const Vector3& rhs) const {
    return Vector3(comps[0] + rhs.comps[0], comps[1] + rhs.comps[1],
                   comps[2] + rhs.comps[2]);
  }
  inline Vector3 operator-(const Vector3& rhs) const {
    return Vector3(comps[0] - rhs.comps[0], comps[1] - rhs.comps[1],
                   comps[2] - rhs.comps[2]);
  }
  inline Vector3 operator*(const Vector3& rhs) const {
    return Vector3(comps[0] * rhs.comps[0], comps[1] * rhs.comps[1],
                   comps[2] * rhs.comps[2]);
  }
  inline Vector3 operator*(double scalar) const {
    return Vector3(comps[0] * scalar, comps[1] * scalar, comps[2] * scalar);
  }
  inline Vector3 operator/(double scalar) const { return *this * (1 / scalar); }
  friend inline Vector3 operator*(double scalar, const Vector3& rhs) {
    return rhs * scalar;
  }
  friend inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    return out << v.comps[0] << ' ' << v.comps[1] << ' ' << v.comps[2];
  }
  friend inline double dot(const Vector3& lhs, const Vector3& rhs) {
    return lhs.comps[0] * rhs.comps[0] + lhs.comps[1] * rhs.comps[1] +
           lhs.comps[2] * rhs.comps[2];
  }
  friend inline Vector3 cross(const Vector3& lhs, const Vector3& rhs) {
    return Vector3(lhs.comps[1] * rhs.comps[2] - lhs.comps[2] * rhs.comps[1],
                   lhs.comps[2] * rhs.comps[0] - lhs.comps[0] * rhs.comps[2],
                   lhs.comps[0] * rhs.comps[1] - lhs.comps[1] * rhs.comps[0]);
  }
};