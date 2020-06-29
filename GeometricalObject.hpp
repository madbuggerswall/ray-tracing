#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "HitRecord.hpp"
#include "Ray.hpp"

class GeometricalObject {
 private:
 public:
  virtual bool hit(const Ray& ray, double tMin, double tMax,
                   HitRecord& hitRecord) const = 0;
};

using GeoObject = GeometricalObject;

#endif