#ifndef ROTATE_HPP
#define ROTATE_HPP

#include "GeometricalObject.hpp"
#include "Math.hpp"

class RotateY : public GeometricalObject {
 private:
  std::shared_ptr<GeometricalObject> object;
  float sinTheta;
  float cosTheta;
  bool hasBox;
  AABB boundingBox;

 public:
  RotateY(std::shared_ptr<GeoObject> object, float angle) : object(object) {
    auto radians = Math::degreesToRadians(angle);
    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
    hasBox = object->computeBoundingBox(0, 1, boundingBox);

    Point3F min(Math::infinity, Math::infinity, Math::infinity);
    Point3F max(-Math::infinity, -Math::infinity, -Math::infinity);

    for (size_t i = 0; i < 2; ++i) {
      for (size_t j = 0; j < 2; ++j) {
        for (size_t k = 0; k < 2; ++k) {
          auto x = i * boundingBox.getMax().x + (1 - i) * boundingBox.getMin().x;
          auto y = j * boundingBox.getMax().y + (1 - j) * boundingBox.getMin().y;
          auto z = k * boundingBox.getMax().z + (1 - k) * boundingBox.getMin().z;

          auto rotatedX = cosTheta * x + sinTheta * z;
          auto rotatedZ = -sinTheta * x + cosTheta * z;

          Vector3F tester(rotatedX, y, rotatedZ);

          for (int c = 0; c < 3; c++) {
            min[c] = std::fmin(min[c], tester[c]);
            max[c] = std::fmax(max[c], tester[c]);
          }
        }
      }
    }
    boundingBox = AABB(min, max);
  }

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    auto origin = ray.origin;
    auto direction = ray.direction;

    origin[0] = cosTheta * ray.origin[0] - sinTheta * ray.origin[2];
    origin[2] = sinTheta * ray.origin[0] + cosTheta * ray.origin[2];

    direction[0] = cosTheta * ray.direction[0] - sinTheta * ray.direction[2];
    direction[2] = sinTheta * ray.direction[0] + cosTheta * ray.direction[2];

    Ray rotatedRay(origin, direction, ray.getTime());

    if (!object->intersect(rotatedRay, tMin, tMax, interaction)) return false;

    auto point = interaction.point;
    auto normal = interaction.normal;

    point[0] = cosTheta * interaction.point[0] + sinTheta * interaction.point[2];
    point[2] = -sinTheta * interaction.point[0] + cosTheta * interaction.point[2];

    normal[0] = cosTheta * interaction.normal[0] + sinTheta * interaction.normal[2];
    normal[2] = -sinTheta * interaction.normal[0] + cosTheta * interaction.normal[2];

    interaction.point = point;
    interaction.setFaceNormal(rotatedRay, normal);

    return true;
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    outputBox = boundingBox;
    return hasBox;
  }
};

#endif
