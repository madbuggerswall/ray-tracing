#ifndef RAY_HPP
#define RAY_HPP

#include "Vector3.hpp"

class Ray {
 private:
  Point3 origin;
  Vector3 direction;

 public:
  Ray() {}
  Ray(const Point3& origin, const Vector3& direction) :
      origin(origin), direction(direction) {}

  Point3 at(double t) const { return origin + t * direction; }

  Point3 getOrigin() const { return origin; }
  Vector3 getDirection() const { return direction; }
};

#endif