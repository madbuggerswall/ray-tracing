#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "AxisAlignedBoundingBox.hpp"
#include "HitRecord.hpp"
#include "Ray.hpp"

class GeometricalObject {
 public:
  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const = 0;
  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const = 0;
  virtual double pdfValue(const Point3& origin, const Vector3& direction) const { return 0.0; }
  virtual Vector3 random(const Vector3& origin) const { return Vector3(1, 0, 0); }
};

using GeoObject = GeometricalObject;

#endif