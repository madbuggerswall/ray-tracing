#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Math/Math.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Point3.hpp"
#include "../Math/Random.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Vector3.hpp"
#include "Configuration.hpp"

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

  double viewportHeight;

 public:
  Camera(Point3 lookFrom, Point3 lookAt, Vector3 viewUp, double vertFov, double aspectRatio, double aperture,
         double focusDist, double time0, double time1) {
    viewportHeight = 2.0 * std::tan(Math::degreesToRadians(vertFov) / 2.0);

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

  Camera(CameraConfiguration config, double time0, double time1) :
      Camera(config.lookFrom, config.lookAt, config.viewUp, config.verticalFOV, config.aspectRatio, config.aperture,
             config.focusDist, time0, time1) {}

  Ray getRay(Point2 sample) const {
    const Vector3 random = lensRadius * Random::vectorInUnitDisk();
    const Vector3 offset = u * random.x + v * random.y;
    const Vector3 direction = lowerLeftCorner + sample.x * horizontal + sample.y * vertical - origin - offset;
    return Ray(origin + offset, direction, Random::range(time0, time1));
  }

  Ray getSample(ushort imageHeight, ushort imageWidth) const {
    const Vector3 su = u * -(0.5 - Random::fraction()) * imageWidth;
    const Vector3 sv = v * (0.5 - Random::fraction()) * imageHeight;
    const Vector3 sw = -w * getDist(imageHeight);
    return Ray(origin, (su + sv + sw).normalized());
  }

  // MLT
  Ray getSample(ushort imageHeight, ushort imageWidth, double random1, double random2) const {
    const Vector3 su = u * -(0.5 - random1) * imageWidth;
    const Vector3 sv = v * (0.5 - random2) * imageHeight;
    const Vector3 sw = -w * getDist(imageHeight);
    return Ray(origin, (su + sv + sw).normalized());
  }

  Vector3 getW() const { return -w; }
  double getDist(const int imageHeight) const { return imageHeight / viewportHeight; }
  Point3 getOrigin() const { return origin; }
  Vector3 getU() const { return u; }
  Vector3 getV() const { return v; }
};

#endif