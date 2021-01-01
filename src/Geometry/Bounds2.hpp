#ifndef BOUNDS2_HPP
#define BOUNDS2_HPP

#include "Point2.hpp"
#include "Ray.hpp"

class Bounds2 {
 public:
  Point2 minPoint, maxPoint;

  Bounds2() {
    float minLimit = std::numeric_limits<float>::lowest();
    float maxLimit = std::numeric_limits<float>::max();
    minPoint = Point2(maxLimit, maxLimit);
    maxPoint = Point2(minLimit, minLimit);
  }
  explicit Bounds2(const Point2& point) : minPoint(point), maxPoint(point) {}
  Bounds2(const Point2& p1, const Point2& p2) : minPoint(min(p1, p2)), maxPoint(max(p1, p2)) {}

  // Member access operators
  const Point2& operator[](int i) const { return (i == 0) ? minPoint : maxPoint; }
  Point2& operator[](int i) { return (i == 0) ? minPoint : maxPoint; }

  // Comparison operators
  bool operator==(const Bounds2& b) const { return b.minPoint == minPoint && b.maxPoint == maxPoint; }
  bool operator!=(const Bounds2& b) const { return b.minPoint != minPoint || b.maxPoint != maxPoint; }

  Vector2 getDiagonal() const { return maxPoint - minPoint; }
  int getMaximumExtent() const {
    Vector2 diagonal = getDiagonal();
    if (diagonal.x > diagonal.y)
      return 0;
    else
      return 1;
  }

  Vector2 offset(const Point2& point) const {
    Vector2 o = point - minPoint;
    if (maxPoint.x > minPoint.x) o.x /= maxPoint.x - minPoint.x;
    if (maxPoint.y > minPoint.y) o.y /= maxPoint.y - minPoint.y;
    return o;
  }

  friend Bounds2 getUnion(const Bounds2& bounds, const Point2& point) {
    return Bounds2(Point2(min(bounds.minPoint, point)), Point2(max(bounds.maxPoint, point)));
  }
  friend Bounds2 getUnion(const Bounds2& b1, const Bounds2& b2) {
    return Bounds2(Point2(min(b1.minPoint, b2.minPoint)), Point2(std::max(b1.maxPoint, b2.maxPoint)));
  }
  friend Bounds2 intersect(const Bounds2& b1, const Bounds2& b2) {
    return Bounds2(Point2(max(b1.minPoint, b2.minPoint)), Point2(min(b1.maxPoint, b2.maxPoint)));
  }
  friend bool overlaps(const Bounds2& b1, const Bounds2& b2) {
    bool x = (b1.maxPoint.x >= b2.minPoint.x) && (b1.minPoint.x <= b2.maxPoint.x);
    bool y = (b1.maxPoint.y >= b2.minPoint.y) && (b1.minPoint.y <= b2.maxPoint.y);
    return (x && y);
  }
  friend bool inside(const Point2& point, const Bounds2& bounds) {
    return (point.x >= bounds.minPoint.x && point.x <= bounds.maxPoint.x && point.y >= bounds.minPoint.y &&
            point.y <= bounds.maxPoint.y);
  }
  friend bool insideExclusive(const Point2& point, const Bounds2& bounds) {
    return (point.x >= bounds.minPoint.x && point.x < bounds.maxPoint.x && point.y >= bounds.minPoint.y &&
            point.y < bounds.maxPoint.y);
  }
  template <typename U>
  friend Bounds2 expand(const Bounds2& bounds, U delta) {
    return Bounds2(bounds.minPoint - Vector2(delta, delta, delta), bounds.maxPoint + Vector2(delta, delta, delta));
  }
};

#endif