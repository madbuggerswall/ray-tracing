#ifndef MOVING_SPHERE_HPP
#define MOVING_SPHERE_HPP

#include "Utilities.hpp"

class AxisAlignedBoundingBox {
 private:
  Point3 min;
  Point3 max;

 public:
  AxisAlignedBoundingBox() {}
  AxisAlignedBoundingBox(const Point3& min, const Point3& max) : min(min), max(max) {}

  Point3 getMin() const { return min; }
  Point3 getMax() const { return max; }

  bool hit(const Ray& ray, double tMin, double tMax) {
    // For each dimension
    for (size_t i = 0; i < 3; i++) {
      auto t0 = std::fmin((min[i] - ray.getOrigin()[i]) / ray.getDirection()[i],
                          (max[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);
      auto t1 = std::fmin((min[i] - ray.getOrigin()[i]) / ray.getDirection()[i],
                          (max[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);

      tMin = std::fmax(t0, tMin);
      tMax = std::fmin(t1, tMax);

      if (tMax <= tMin) return false;
    }
    return true;
  }

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
};

#endif