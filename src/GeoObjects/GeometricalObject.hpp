#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "../Core/AxisAlignedBoundingBox.hpp"
#include "../Core/SurfaceInteraction.hpp"
#include "../Math/Ray.hpp"

class GeometricalObject {
 public:
  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const = 0;
  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const = 0;
  virtual Point3 samplePoint() const { return Point3(0, 0, 0); }
  virtual float getArea() const { return 0; }
  virtual std::shared_ptr<Material> getMaterial() const { return nullptr; }
};

using GeoObject = GeometricalObject;

#endif