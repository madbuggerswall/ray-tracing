#ifndef ONB_H
#define ONB_H

#include "../Math/Vector3.hpp"

class OrthonormalBasis {
 public:
  Vector3 u, v, w;
  OrthonormalBasis(const Vector3 normal) {
    w = normal.normalized();
    Vector3 a = (fabs(w.x) > 0.9) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    v = cross(w, a).normalized();
    u = cross(w, v);
  }

  Vector3 local(double a, double b, double c) const { return a * u + b * v + c * w; }
  Vector3 local(const Vector3& a) const { return a.x * u + a.y * v + a.z * w; }
};

using ONB = OrthonormalBasis;
#endif