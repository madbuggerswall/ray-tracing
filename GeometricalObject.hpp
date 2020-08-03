#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "AxisAlignedBoundingBox.hpp"
#include "HitRecord.hpp"
#include "Ray.hpp"

class GeometricalObject {
 public:
  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const = 0;
  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const = 0;
};

using GeoObject = GeometricalObject;

#endif