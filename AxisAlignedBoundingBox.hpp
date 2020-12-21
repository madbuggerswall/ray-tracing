#ifndef AXIS_ALIGNED_BOUNDING_BOX_HPP
#define AXIS_ALIGNED_BOUNDING_BOX_HPP

#include "Geometry/Point3.hpp"
// #include "Geometry/Ray.hpp"
#include "Geometry/Ray.hpp"
#include "Geometry/Vector3.hpp"

class AxisAlignedBoundingBox {
  using AABB = AxisAlignedBoundingBox;

 private:
  Point3F min;
  Point3F max;

 public:
  AxisAlignedBoundingBox() {}
  AxisAlignedBoundingBox(const Point3F& min, const Point3F& max) : min(min), max(max) {}

  Point3F getMin() const { return min; }
  Point3F getMax() const { return max; }

  inline bool hit(const Ray& ray, float tMin, float tMax) const {
    for (int a = 0; a < 3; a++) {
      auto inverseDirection = 1.0f / ray.direction[a];
      auto t0 = (min[a] - ray.origin[a]) * inverseDirection;
      auto t1 = (max[a] - ray.origin[a]) * inverseDirection;
      if (inverseDirection < 0.0f) std::swap(t0, t1);
      tMin = t0 > tMin ? t0 : tMin;
      tMax = t1 < tMax ? t1 : tMax;
      if (tMax <= tMin) return false;
    }
    return true;
  }

  static AABB surroundingBox(AABB box0, AABB box1) {
    auto xMin = fmin(box0.getMin().x, box1.getMin().x);
    auto yMin = fmin(box0.getMin().y, box1.getMin().y);
    auto zMin = fmin(box0.getMin().z, box1.getMin().z);
    auto xMax = fmax(box0.getMax().x, box1.getMax().x);
    auto yMax = fmax(box0.getMax().y, box1.getMax().y);
    auto zMax = fmax(box0.getMax().z, box1.getMax().z);

    Point3F small(xMin, yMin, zMin);
    Point3F big(xMax, yMax, zMax);

    return AABB(small, big);
  }
};

using AABB = AxisAlignedBoundingBox;
#endif