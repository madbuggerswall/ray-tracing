#ifndef RAY_HPP
#define RAY_HPP

#include "Geometry/Point3.hpp"
#include "Geometry/Vector3.hpp"

class Ray {
 private:
  Point3F origin;
  Vector3F direction;
  float time;

 public:
  Ray() {}
  Ray(const Point3F& origin, const Vector3F& direction, float time = 0.0) :
      origin(origin),
      direction(direction),
      time(time) {}

  // P(t) = A + t * b
  Point3F at(float t) const { return origin + t * direction; }
  Point3F getOrigin() const { return origin; }
  Vector3F getDirection() const { return direction; }

  float getTime() const { return time; }
};

#endif