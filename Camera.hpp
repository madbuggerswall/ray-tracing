#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Utilities.hpp"

class Camera {
 private:
  Point3 origin;
  Point3 lowerLeftCorner;
  Vector3 horizontal;
  Vector3 vertical;
  Vector3 u, v, w;
  double lensRadius;
  double time0;
  double time1;

 public:
  Camera(
      Point3 lookFrom,
      Point3 lookAt,
      Vector3 viewUp,
      double vertFov,
      double aspectRatio,
      double aperture,
      double focusDist,
      double time0,
      double time1) {
    auto viewportHeight = 2.0 * std::tan(Math::degreesToRadians(vertFov) / 2.0);
    auto viewportWidth = viewportHeight * aspectRatio;
    auto focalLength = 1.0;

    w = (lookFrom - lookAt).normalized();
    u = cross(viewUp, w).normalized();
    v = cross(w, u);

    origin = lookFrom;
    horizontal = focusDist * viewportWidth * u;
    vertical = focusDist * viewportHeight * v;
    lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDist * w;

    lensRadius = aperture / 2;
    this->time0 = time0;
    this->time1 = time1;
  }

  Ray getRay(double s, double t) const {
    Vector3 random = lensRadius * Random::vectorInUnitDisk();
    Vector3 offset = u * random.getX() + v * random.getY();

    return Ray(
        origin + offset,
        lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
        Random::range(time0, time1));
  }
};

#endif