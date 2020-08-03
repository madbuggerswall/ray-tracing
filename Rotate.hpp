#ifndef ROTATE_HPP
#define ROTATE_HPP

#include "GeometricalObject.hpp"
#include "Utilities.hpp"

class RotateY : public GeometricalObject {
 private:
  std::shared_ptr<GeometricalObject> object;
  double sinTheta;
  double cosTheta;
  bool hasBox;
  AABB boundingBox;

 public:
  RotateY(std::shared_ptr<GeoObject> object, double angle) : object(object) {
    auto radians = Math::degreesToRadians(angle);
    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
    hasBox = object->computeBoundingBox(0, 1, boundingBox);

    Point3 min(Math::infinity, Math::infinity, Math::infinity);
    Point3 max(-Math::infinity, -Math::infinity, -Math::infinity);

    for (size_t i = 0; i < 2; ++i) {
      for (size_t j = 0; j < 2; ++j) {
        for (size_t k = 0; k < 2; ++k) {
          auto x = i * boundingBox.getMax().getX() + (1 - i) * boundingBox.getMin().getX();
          auto y = j * boundingBox.getMax().getY() + (1 - j) * boundingBox.getMin().getY();
          auto z = k * boundingBox.getMax().getZ() + (1 - k) * boundingBox.getMin().getZ();

          auto rotatedX = cosTheta * x + sinTheta * z;
          auto rotatedZ = -sinTheta * x + cosTheta * z;

          Vector3 tester(rotatedX, y, rotatedZ);

          for (int c = 0; c < 3; c++) {
            min[c] = std::fmin(min[c], tester[c]);
            max[c] = std::fmax(max[c], tester[c]);
          }
        }
      }
    }
    boundingBox = AABB(min, max);
  }

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    auto origin = ray.getOrigin();
    auto direction = ray.getDirection();

    origin[0] = cosTheta * ray.getOrigin()[0] - sinTheta * ray.getOrigin()[2];
    origin[2] = sinTheta * ray.getOrigin()[0] + cosTheta * ray.getOrigin()[2];

    direction[0] = cosTheta * ray.getDirection()[0] - sinTheta * ray.getDirection()[2];
    direction[2] = sinTheta * ray.getDirection()[0] + cosTheta * ray.getDirection()[2];

    Ray rotatedRay(origin, direction, ray.getTime());

    if (!object->hit(rotatedRay, tMin, tMax, hitRecord)) return false;

    auto point = hitRecord.point;
    auto normal = hitRecord.normal;

    point[0] = cosTheta * hitRecord.point[0] + sinTheta * hitRecord.point[2];
    point[2] = -sinTheta * hitRecord.point[0] + cosTheta * hitRecord.point[2];

    normal[0] = cosTheta * hitRecord.normal[0] + sinTheta * hitRecord.normal[2];
    normal[2] = -sinTheta * hitRecord.normal[0] + cosTheta * hitRecord.normal[2];

    hitRecord.point = point;
    hitRecord.setFaceNormal(rotatedRay, normal);

    return true;
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    outputBox = boundingBox;
    return hasBox;
  }
};

#endif
