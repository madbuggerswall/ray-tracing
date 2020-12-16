#ifndef BOUNDS2_HPP
#define BOUNDS2_HPP

#include "Point2.hpp"
#include "Ray.hpp"

template <typename T>
class Bounds2 {
 public:
  Point2<T> minPoint, maxPoint;

  Bounds2() {
    T minLimit = std::numeric_limits<T>::lowest();
    T maxLimit = std::numeric_limits<T>::maxPoint();
    minPoint = Point2<T>(maxLimit, maxLimit, maxLimit);
    maxPoint = Point2<T>(minLimit, minLimit, minLimit);
  }
  explicit Bounds2(const Point2<T> &point) : minPoint(point), maxPoint(point) {}
  Bounds2(const Point2<T> &p1, const Point2<T> &p2) : minPoint(min(p1, p2)), maxPoint(max(p1, p2)) {}

  // Cast operator
  template <typename U>
  explicit operator Bounds2<U>() const {
    return Bounds2<U>((Point2<U>) minPoint, (Point2<U>) maxPoint);
  }

  // Member access operators
  const Point2<T> &operator[](int i) const { return (i == 0) ? minPoint : maxPoint; }
  Point2<T> &operator[](int i) { return (i == 0) ? minPoint : maxPoint; }

  // Comparison operators
  bool operator==(const Bounds2<T> &b) const { return b.minPoint == minPoint && b.maxPoint == maxPoint; }
  bool operator!=(const Bounds2<T> &b) const { return b.minPoint != minPoint || b.maxPoint != maxPoint; }

  Vector2<T> getDiagonal() const { return maxPoint - minPoint; }
  int getMaximumExtent() const {
    Vector2<T> diagonal = getDiagonal();
    if (diagonal.x > diagonal.y)
      return 0;
    else
      return 1;
  }

  Point2<T> lerp(const Point2F &t) const {
    return Point2<T>(lerp(t.x, minPoint.x, maxPoint.x), lerp(t.y, minPoint.y, maxPoint.y));
  }
  Vector2<T> offset(const Point2<T> &point) const {
    Vector2<T> o = point - minPoint;
    if (maxPoint.x > minPoint.x) o.x /= maxPoint.x - minPoint.x;
    if (maxPoint.y > minPoint.y) o.y /= maxPoint.y - minPoint.y;
    return o;
  }

  friend Bounds2 getUnion(const Bounds2 &bounds, const Point2<T> &point) {
    return Bounds2(Point2<T>(min(bounds.minPoint, point)), Point2<T>(max(bounds.maxPoint, point)));
  }
  friend Bounds2 getUnion(const Bounds2 &b1, const Bounds2<T> &b2) {
    return Bounds2(Point2<T>(min(b1.minPoint, b2.minPoint)), Point2<T>(std::max(b1.maxPoint, b2.maxPoint)));
  }
  friend Bounds2 intersect(const Bounds2 &b1, const Bounds2<T> &b2) {
    return Bounds2(Point2<T>(max(b1.minPoint, b2.minPoint)), Point2<T>(min(b1.maxPoint, b2.maxPoint)));
  }
  friend bool overlaps(const Bounds2 &b1, const Bounds2<T> &b2) {
    bool x = (b1.maxPoint.x >= b2.minPoint.x) && (b1.minPoint.x <= b2.maxPoint.x);
    bool y = (b1.maxPoint.y >= b2.minPoint.y) && (b1.minPoint.y <= b2.maxPoint.y);
    return (x && y);
  }
  friend bool inside(const Point2<T> &point, const Bounds2<T> &bounds) {
    return (point.x >= bounds.minPoint.x && point.x <= bounds.maxPoint.x && point.y >= bounds.minPoint.y &&
            point.y <= bounds.maxPoint.y);
  }
  friend bool insideExclusive(const Point2<T> &point, const Bounds2<T> &bounds) {
    return (point.x >= bounds.minPoint.x && point.x < bounds.maxPoint.x && point.y >= bounds.minPoint.y &&
            point.y < bounds.maxPoint.y);
  }
  template <typename U>
  friend Bounds2<T> expand(const Bounds2<T> &bounds, U delta) {
    return Bounds2<T>(bounds.minPoint - Vector2<T>(delta, delta, delta),
                      bounds.maxPoint + Vector2<T>(delta, delta, delta));
  }
};

using Bounds2F = Bounds2<float>;
using Bounds2Int = Bounds2<int>;

#endif