#ifndef RAY_HPP
#define RAY_HPP

#include "../Core/Math.hpp"
#include "Point3.hpp"
#include "Vector3.hpp"

class Ray {
 public:
  Point3F origin;
  Vector3F direction;
  float tMin, tMax;
  float time;

  Ray() {}
  Ray(const Point3F& origin, const Vector3F& direction, float time = 0.0) :
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
  Ray(Ray&& other) :
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
  Ray& operator=(Ray&& other) {
    direction = std::move(other.direction);
    origin = std::move(other.origin);
    tMin = other.tMin;
    tMax = other.tMax;
    time = other.time;
    return *this;
  }

  Point3F operator()(float t) { return origin + t * direction; }

  // P(t) = O+ t * d
  Point3F at(float t) const { return origin + t * direction; }
  Point3F getOrigin() const { return origin; }
  Vector3F getDirection() const { return direction; }

  float getTime() const { return time; }
};

#endif