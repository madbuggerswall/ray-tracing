#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "Geometry/Point3.hpp"
#include "Geometry/Vector3.hpp"
#include "Ray.hpp"

class Material;
struct HitRecord {
  Point3F point;
  Vector3F normal;
  UV uv;
  float t;
  bool frontFace;
  std::shared_ptr<Material> materialPtr;

  inline void setFaceNormal(const Ray& ray, const Vector3F outwardNormal) {
    frontFace = dot(ray.getDirection(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

#endif