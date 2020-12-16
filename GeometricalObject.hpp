#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "AxisAlignedBoundingBox.hpp"
#include "HitRecord.hpp"
#include "Ray.hpp"

class GeometricalObject {
 public:
  virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const = 0;
  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const = 0;
};

using GeoObject = GeometricalObject;

#endif