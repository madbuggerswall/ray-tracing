#ifndef AXIS_ALIGNED_BOUNDING_BOX_HPP
#define AXIS_ALIGNED_BOUNDING_BOX_HPP

#include "Utilities.hpp"

class AxisAlignedBoundingBox {
  using AABB = AxisAlignedBoundingBox;

 private:
  Point3 min;
  Point3 max;

 public:
  AxisAlignedBoundingBox() {}
  AxisAlignedBoundingBox(const Point3& min, const Point3& max) : min(min), max(max) {}

  Point3 getMin() const { return min; }
  Point3 getMax() const { return max; }

  inline bool hit(const Ray& ray, double tMin, double tMax) const {
    for (int a = 0; a < 3; a++) {
      auto inverseDirection = 1.0f / ray.getDirection()[a];
      auto t0 = (min[a] - ray.getOrigin()[a]) * inverseDirection;
      auto t1 = (max[a] - ray.getOrigin()[a]) * inverseDirection;
      if (inverseDirection < 0.0f) std::swap(t0, t1);
      tMin = t0 > tMin ? t0 : tMin;
      tMax = t1 < tMax ? t1 : tMax;
      if (tMax <= tMin) return false;
    }
    return true;
  }

  static AABB surroundingBox(AABB box0, AABB box1) {
    auto xMin = fmin(box0.getMin().getX(), box1.getMin().getX());
    auto yMin = fmin(box0.getMin().getY(), box1.getMin().getY());
    auto zMin = fmin(box0.getMin().getZ(), box1.getMin().getZ());
    auto xMax = fmax(box0.getMax().getX(), box1.getMax().getX());
    auto yMax = fmax(box0.getMax().getY(), box1.getMax().getY());
    auto zMax = fmax(box0.getMax().getZ(), box1.getMax().getZ());

    Point3 small(xMin, yMin, zMin);
    Point3 big(xMax, yMax, zMax);

    return AABB(small, big);
  }
};

using AABB = AxisAlignedBoundingBox;
#endif