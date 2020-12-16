#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Geometry/Point3.hpp"
#include "Geometry/Vector3.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Ray.hpp"

class Camera {
 private:
  Point3F origin;
  Point3F lowerLeftCorner;
  Vector3F horizontal;
  Vector3F vertical;
  Vector3F u, v, w;
  float lensRadius;
  float time0;
  float time1;

 public:
  Camera(Point3F lookFrom, Point3F lookAt, Vector3F viewUp, float vertFov, float aspectRatio, float aperture,
         float focusDist, float time0, float time1) {
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

  Ray getRay(float s, float t) const {
    Vector3F random = lensRadius * Random::vectorInUnitDisk();
    Vector3F offset = u * random.x + v * random.y;

    return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
               Random::range(time0, time1));
  }
};

#endif