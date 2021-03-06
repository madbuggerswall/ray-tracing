#ifndef AXIS_ALIGNED_BOUNDING_BOX_HPP
#define AXIS_ALIGNED_BOUNDING_BOX_HPP

#include "../Math/Point3.hpp"
// #include "Geometry/Ray.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Vector3.hpp"

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

  inline bool intersect(const Ray& ray, double tMin, double tMax) const {
    for (int a = 0; a < 3; a++) {
      const auto inverseDirection = 1.0f / ray.direction[a];
      auto t0 = (min[a] - ray.origin[a]) * inverseDirection;
      auto t1 = (max[a] - ray.origin[a]) * inverseDirection;
      if (inverseDirection < 0.0f) std::swap(t0, t1);
      tMin = t0 > tMin ? t0 : tMin;
      tMax = t1 < tMax ? t1 : tMax;
      if (tMax <= tMin) return false;
    }
    return true;
  }

  static AABB surroundingBox(const AABB& box0, const AABB& box1) {
    const auto xMin = fmin(box0.getMin().x, box1.getMin().x);
    const auto yMin = fmin(box0.getMin().y, box1.getMin().y);
    const auto zMin = fmin(box0.getMin().z, box1.getMin().z);
    const auto xMax = fmax(box0.getMax().x, box1.getMax().x);
    const auto yMax = fmax(box0.getMax().y, box1.getMax().y);
    const auto zMax = fmax(box0.getMax().z, box1.getMax().z);

    const Point3 small(xMin, yMin, zMin);
    const Point3 big(xMax, yMax, zMax);

    return AABB(small, big);
  }
};

using AABB = AxisAlignedBoundingBox;
#endif