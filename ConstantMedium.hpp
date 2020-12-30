#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "GeometricalObject.hpp"
#include "Isotropic.hpp"
#include "Material.hpp"
#include "Texture.hpp"


class ConstantMedium : public GeometricalObject {
 private:
  std::shared_ptr<GeometricalObject> shape;
  std::shared_ptr<Material> phaseFunction;
  float negInvDensity;

 public:
  ConstantMedium(std::shared_ptr<GeoObject> shape, float density, std::shared_ptr<Texture> albedo) :
      shape(shape),
      negInvDensity(-1 / density),
      phaseFunction(std::make_shared<Isotropic>(albedo)) {}
  ConstantMedium(std::shared_ptr<GeoObject> shape, float density, Color color) :
      shape(shape),
      negInvDensity(-1 / density),
      phaseFunction(std::make_shared<Isotropic>(color)) {}

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    SInteraction hitRecA, hitRecB;

    if (!shape->intersect(ray, -Math::infinity, Math::infinity, hitRecA)) return false;

    if (!shape->intersect(ray, hitRecA.t + 0.0001, Math::infinity, hitRecB)) return false;

    if (hitRecA.t < tMin) hitRecA.t = tMin;
    if (hitRecB.t > tMax) hitRecB.t = tMax;

    if (hitRecA.t >= hitRecB.t) return false;

    if (hitRecA.t < 0) hitRecA.t = 0;

    const auto rayLength = ray.direction.magnitude();
    const auto distanceInsideShape = (hitRecB.t - hitRecA.t) * rayLength;
    const auto hitDistance = negInvDensity * std::log(Random::fraction());

    if (hitDistance > distanceInsideShape) return false;

    interaction.t = hitRecA.t + hitDistance / rayLength;
    interaction.point = ray.at(interaction.t);

    interaction.normal = Vector3F(1, 0, 0);  // arbitrary
    interaction.frontFace = true;           // also arbitrary
    interaction.materialPtr = phaseFunction;

    return true;
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    return shape->computeBoundingBox(t0, t1, outputBox);
  }
};

#endif