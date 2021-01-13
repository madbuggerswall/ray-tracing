#ifndef GEOMETRICAL_OBJECT_HPP
#define GEOMETRICAL_OBJECT_HPP

#include "../Core/AxisAlignedBoundingBox.hpp"
#include "../Core/SurfaceInteraction.hpp"
#include "../Math/Ray.hpp"

class GeometricalObject {
 public:
  virtual bool intersect(const Ray& ray, double tMin, double tMax, SInteraction& interaction) const = 0;
  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const = 0;
  virtual Point3 samplePoint() const { return Point3(0, 0, 0); }
  virtual Point3 samplePoint(double random1, double random2) const { return Point3(0, 0, 0); }
  virtual Ray sampleDirection() const { return Ray(); }
  virtual Ray sampleDirection(double random1, double random2, double, double) const { return Ray(); }
  virtual double getArea() const { return 0; }
  virtual std::shared_ptr<Material> getMaterial() const { return nullptr; }
};

using GeoObject = GeometricalObject;

#endif