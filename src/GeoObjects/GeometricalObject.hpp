#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "../Core/AxisAlignedBoundingBox.hpp"
#include "../Core/SurfaceInteraction.hpp"
#include "../Geometry/Ray.hpp"

class GeometricalObject {
 public:
  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const = 0;
  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const = 0;
};

using GeoObject = GeometricalObject;

#endif