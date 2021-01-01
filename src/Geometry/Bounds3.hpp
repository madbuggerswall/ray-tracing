#ifndef BOUNDS3_HPP
#define BOUNDS3_HPP

#include "Point3.hpp"
#include "Ray.hpp"

class Bounds3 {
 public:
  Point3 minPoint, maxPoint;

  Bounds3() {
    float minLimit = std::numeric_limits<float>::lowest();
    float maxLimit = std::numeric_limits<float>::max();
    minPoint = Point3(maxLimit, maxLimit, maxLimit);
    maxPoint = Point3(minLimit, minLimit, minLimit);
  }

  // Copy constructor
  Bounds3(const Bounds3& other) : minPoint(other.minPoint), maxPoint(other.maxPoint) {}

  // Move constructor
  Bounds3(Bounds3&& other) : minPoint(std::move(other.minPoint)), maxPoint(std::move(other.maxPoint)) {}

  // Copy assignment
  Bounds3& operator=(const Bounds3& other) {
    minPoint = other.minPoint;
    maxPoint = other.maxPoint;
    return *this;
  }

  // Move assignment
  Bounds3& operator=(Bounds3&& other) {
    minPoint = std::move(other.minPoint);
    maxPoint = std::move(other.maxPoint);
    return *this;
  }

  explicit Bounds3(const Point3& point) : minPoint(point), maxPoint(point) {}
  Bounds3(const Point3& p1, const Point3& p2) : minPoint(min(p1, p2)), maxPoint(max(p1, p2)) {}

  const Point3& operator[](int i) const;
  Point3& operator[](int i);

  bool operator==(const Bounds3& b) const { return b.minPoint == minPoint && b.maxPoint == maxPoint; }
  bool operator!=(const Bounds3& b) const { return b.minPoint != minPoint || b.maxPoint != maxPoint; }

  Point3 getCorner(int corner) const {
    return Point3((*this)[(corner & 1)].x, (*this)[(corner & 2) ? 1 : 0].y, (*this)[(corner & 4) ? 1 : 0].z);
  }
  Vector3 getDiagonal() const { return maxPoint - minPoint; }

  float getSurfaceArea() const {
    Vector3 diagonal = getDiagonal();
    return 2 * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
  }
  float getVolume() const {
    Vector3 diagonal = getDiagonal();
    return diagonal.x * diagonal.y * diagonal.z;
  }
  int getMaximumExtent() const {
    Vector3 diagonal = getDiagonal();
    if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
      return 0;
    else if (diagonal.y > diagonal.z)
      return 1;
    else
      return 2;
  }

  Vector3 offset(const Point3& point) const {
    Vector3 o = point - minPoint;
    if (maxPoint.x > minPoint.x) o.x /= maxPoint.x - minPoint.x;
    if (maxPoint.y > minPoint.y) o.y /= maxPoint.y - minPoint.y;
    if (maxPoint.z > minPoint.z) o.z /= maxPoint.z - minPoint.z;
    return o;
  }

  bool intersectP(const Ray& ray, float* hitt0 = nullptr, float* hitt1 = nullptr) const {
    float t0 = 0, t1 = ray.tMax;
    for (int i = 0; i < 3; ++i) {
      // Update interval for ith bounding box slab.
      float invRayDir = 1 / ray.direction[i];
      float tNear = (minPoint[i] - ray.origin[i]) * invRayDir;
      float tFar = (maxPoint[i] - ray.origin[i]) * invRayDir;
      // Update parametric interval from slab intersection values.
      if (tNear > tFar) std::swap(tNear, tFar);
      // [EXCLUDED] Update tFar to ensure robust ray–bounds intersection. Gamma calls excluded.

      t0 = tNear > t0 ? tNear : t0;
      t1 = tFar < t1 ? tFar : t1;
      if (t0 > t1) return false;
    }
    if (hitt0) *hitt0 = t0;
    if (hitt1) *hitt1 = t1;
    return true;
  }
  bool intersectP(const Ray& ray, const Vector3F& invDir, const int dirIsNeg[3]) const {
    const Bounds3& bounds = *this;
    // <<Check for ray intersection against  and  slabs>>
    float tMin = (bounds[dirIsNeg[0]].x - ray.origin.x) * invDir.x;
    float tMax = (bounds[1 - dirIsNeg[0]].x - ray.origin.x) * invDir.x;
    float tyMin = (bounds[dirIsNeg[1]].y - ray.origin.y) * invDir.y;
    float tyMax = (bounds[1 - dirIsNeg[1]].y - ray.origin.y) * invDir.y;
    // [EXCLUDED] Update tMax and tyMax to ensure robust bounds intersection. Excluded gamma calls.

    if (tMin > tyMax || tyMin > tMax) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    // <<Check for ray intersection against  slab>>
    float tzMin = (bounds[dirIsNeg[2]].z - ray.origin.z) * invDir.z;
    float tzMax = (bounds[1 - dirIsNeg[2]].z - ray.origin.z) * invDir.z;
    //  [EXCLUDED] Update tzMax to ensure robust bounds intersection. Excluded gamma calls.

    if (tMin > tzMax || tzMin > tMax) return false;
    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;

    return (tMin < ray.tMax) && (tMax > 0);
  }

  friend Bounds3 getUnion(const Bounds3& bounds, const Point3& point) {
    return Bounds3(Point3(min(bounds.minPoint, point)), Point3(max(bounds.maxPoint, point)));
  }
  friend Bounds3 getUnion(const Bounds3& b1, const Bounds3& b2) {
    return Bounds3(Point3(min(b1.minPoint, b2.minPoint)), Point3(std::max(b1.maxPoint, b2.maxPoint)));
  }
  friend Bounds3 intersect(const Bounds3& b1, const Bounds3& b2) {
    return Bounds3(Point3(max(b1.minPoint, b2.minPoint)), Point3(min(b1.maxPoint, b2.maxPoint)));
  }
  friend bool overlaps(const Bounds3& b1, const Bounds3& b2) {
    bool x = (b1.maxPoint.x >= b2.minPoint.x) && (b1.minPoint.x <= b2.maxPoint.x);
    bool y = (b1.maxPoint.y >= b2.minPoint.y) && (b1.minPoint.y <= b2.maxPoint.y);
    bool z = (b1.maxPoint.z >= b2.minPoint.z) && (b1.minPoint.z <= b2.maxPoint.z);
    return (x && y && z);
  }
  friend bool inside(const Point3& point, const Bounds3& bounds) {
    return (point.x >= bounds.minPoint.x && point.x <= bounds.maxPoint.x && point.y >= bounds.minPoint.y &&
            point.y <= bounds.maxPoint.y && point.z >= bounds.minPoint.z && point.z <= bounds.maxPoint.z);
  }
  friend bool insideExclusive(const Point3& point, const Bounds3& bounds) {
    return (point.x >= bounds.minPoint.x && point.x < bounds.maxPoint.x && point.y >= bounds.minPoint.y &&
            point.y < bounds.maxPoint.y && point.z >= bounds.minPoint.z && point.z < bounds.maxPoint.z);
  }
  template <typename U>
  inline Bounds3 expand(const Bounds3& bounds, U delta) {
    return Bounds3(bounds.minPoint - Vector3(delta, delta, delta), bounds.maxPoint + Vector3(delta, delta, delta));
  }
};

#endif