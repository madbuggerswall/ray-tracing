#ifndef RAY_HPP
#define RAY_HPP

#include "Math.hpp"
#include "Point3.hpp"
#include "Vector3.hpp"

class Ray {
 public:
  Point3 origin;
  Vector3 direction;
  double tMin, tMax;
  double time;

  Ray() {}
  Ray(const Point3& origin, const Vector3& direction, double time = 0.0) :
      origin(origin),
      direction(direction),
      time(time) {}

  // Copy constructor
  Ray(const Ray& other) :
      origin(other.origin),
      direction(other.direction),
      tMin(other.tMin),
      tMax(other.tMax),
      time(other.time) {}

  // Move constructor
  Ray(Ray&& other) noexcept :
      origin(std::move(other.origin)),
      direction(std::move(other.direction)),
      tMin(std::exchange(other.tMin, 0)),
      tMax(std::exchange(other.tMax, 0)),
      time(std::exchange(other.time, 0)) {}

  // Copy assignment
  Ray& operator=(const Ray& other) {
    origin = other.origin;
    direction = other.direction;
    tMin = other.tMin;
    tMax = other.tMax;
    time = other.time;
    return *this;
  }

  // Move assignment
  Ray& operator=(Ray&& other) noexcept {
    direction = std::move(other.direction);
    origin = std::move(other.origin);
    tMin = other.tMin;
    tMax = other.tMax;
    time = other.time;
    return *this;
  }

  Point3 operator()(double t) { return origin + t * direction; }

  // P(t) = O+ t * d
  Point3 at(double t) const { return origin + t * direction; }
  Point3 getOrigin() const { return origin; }
  Vector3 getDirection() const { return direction; }

  double getTime() const { return time; }
};

#endif