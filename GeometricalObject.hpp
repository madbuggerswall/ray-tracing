#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "Ray.hpp"

struct HitRecord {
  Point3 point;
  Vector3 normal;
  double t;
  bool frontFace;

  inline void setFaceNormal(const Ray& ray, const Vector3 outwardNormal) {
    frontFace = dot(ray.getDirection(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

class GeometricalObject {
 private:
 public:
  virtual bool hit(const Ray& ray,
                   double tMin,
                   double tMax,
                   HitRecord& hitRecord) const = 0;
};

using GeoObject = GeometricalObject;

#endif