#ifndef RAY_HPP
#define RAY_HPP

#include "Vector3.hpp"

class Ray {
 private:
  Point3 origin;
  Vector3 direction;
  double time;

 public:
  Ray() {}
  Ray(const Point3& origin, const Vector3& direction, double time = 0.0) :
      origin(origin),
      direction(direction),
      time(time) {}

	// P(t) = A + t * b
  Point3 at(double t) const {
    return origin + t * direction;
  }
  Point3 getOrigin() const {
    return origin;
  }
  Vector3 getDirection() const {
    return direction;
  }

  double getTime() const {
    return time;
  }
};

#endif