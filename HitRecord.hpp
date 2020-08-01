#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "Utilities.hpp"

class Material;
struct HitRecord {
  Point3 point;
  Vector3 normal;
  UV uv;
  double t;
  bool frontFace;
  std::shared_ptr<Material> materialPtr;

  inline void setFaceNormal(const Ray& ray, const Vector3 outwardNormal) {
    frontFace = dot(ray.getDirection(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

#endif