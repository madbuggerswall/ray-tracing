#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Configuration.hpp"
#include "Geometry/Point2.hpp"
#include "Geometry/Point3.hpp"
#include "Geometry/Ray.hpp"
#include "Geometry/Vector3.hpp"
#include "Math.hpp"
#include "Random.hpp"

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
    const auto viewportHeight = 2.0 * std::tan(Math::degreesToRadians(vertFov) / 2.0);
    const auto viewportWidth = viewportHeight * aspectRatio;
    const auto focalLength = 1.0;

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

  Camera(CameraConfiguration config, float time0, float time1) :
      Camera(config.lookFrom, config.lookAt, config.viewUp, config.verticalFOV, config.aspectRatio, config.aperture,
             config.focusDist, time0, time1) {}

  Ray getRay(Point2F sample) const {
    const Vector3F random = lensRadius * Random::vectorInUnitDisk();
    const Vector3F offset = u * random.x + v * random.y;
    const Vector3 direction = lowerLeftCorner + sample.x * horizontal + sample.y * vertical - origin - offset;
    return Ray(origin + offset, direction, Random::range(time0, time1));
  }
};

#endif