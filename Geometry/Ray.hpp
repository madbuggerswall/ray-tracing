// #ifndef RAY_HPP
// #define RAY_HPP

// #include "../Math.hpp"
// #include "Point3.hpp"
// #include "Vector3.hpp"

// class Ray {
//  public:
//   Point3F origin;
//   Vector3F direction;
//   float tMin, tMax;
//   float time;

//   Ray() : tMax(Math::infinity), time(0.f) {}
//   Ray(const Point3F& origin, const Vector3F& direction, float tMax = Math::infinity, float time = 0.0) :
//       origin(origin),
//       direction(direction),
//       tMax(tMax),
//       time(time) {}

//   Point3F operator()(float t) { return origin + t * direction; }

//   // P(t) = O+ t * d
//   Point3F at(float t) const { return origin + t * direction; }
//   Point3F getOrigin() const { return origin; }
//   Vector3F getDirection() const { return direction; }

//   float getTime() const { return time; }
// };

// #endif