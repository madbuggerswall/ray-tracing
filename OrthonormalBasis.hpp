#ifndef ORTHONORMALBASIS_HPP
#define ORTHONORMALBASIS_HPP

#include "Utilities.hpp"

class OrthonormalBasis {
 private:
  Vector3 axes[3];

 public:
  OrthonormalBasis() {}
  OrthonormalBasis(const Vector3& normal) { buildFromW(normal); }

  // Member functions are implicitly inline.
  Vector3 operator[](int index) const { return axes[index]; }
  Vector3 getU() const { return axes[0]; }
  Vector3 getV() const { return axes[1]; }
  Vector3 getW() const { return axes[2]; }

  Vector3 local(double a, double b, double c) const { return a * axes[0] + b * axes[1] + c * axes[2]; }

  Vector3 local(const Vector3& a) const { return a.getX() * axes[0] + a.getY() * axes[1] + a.getZ() * axes[2]; }

  void buildFromW(const Vector3& normal) {
    axes[2] = normal.normalized();
    Vector3 arbitraryAxis = (std::fabs(getW().getX()) > 0.9) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    axes[1] = cross(getW(), arbitraryAxis).normalized();
    axes[0] = cross(getW(), getV());
  }
};

using ONB = OrthonormalBasis;

#endif