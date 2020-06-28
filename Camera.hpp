#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Utilities.hpp"

class Camera {
 private:
  Point3 origin;
  Point3 lowerLeftCorner;
  Vector3 horizontal;
  Vector3 vertical;
  const double aspectRatio = 16.0 / 9.0;

 public:
  Camera() {
    auto viewportHeight = 2.0;
    auto viewportWidth = viewportHeight * aspectRatio;
    auto focalLength = 1.0;

    origin = Point3(0, 0, 0);
    horizontal = Vector3(viewportWidth, 0, 0);
    vertical = Vector3(0, viewportHeight, 0);
    lowerLeftCorner = origin - horizontal / 2 - vertical / 2;
    lowerLeftCorner -= Vector3(0, 0, focalLength);
  }

  Ray getRay(double u, double v) const {
    return Ray(origin,
               lowerLeftCorner + u * horizontal + v * vertical - origin);
  }
};

#endif