#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "../Materials/Isotropic.hpp"
#include "../Materials/Material.hpp"
#include "../Textures/Texture.hpp"
#include "GeometricalObject.hpp"

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
    SInteraction interactionA, interactionB;

    if (!shape->intersect(ray, -Math::infinity, Math::infinity, interactionA)) return false;

    if (!shape->intersect(ray, interactionA.t + 0.0001, Math::infinity, interactionB)) return false;

    if (interactionA.t < tMin) interactionA.t = tMin;
    if (interactionB.t > tMax) interactionB.t = tMax;

    if (interactionA.t >= interactionB.t) return false;

    if (interactionA.t < 0) interactionA.t = 0;

    const auto rayLength = ray.direction.magnitude();
    const auto distanceInsideShape = (interactionB.t - interactionA.t) * rayLength;
    const auto hitDistance = negInvDensity * std::log(Random::fraction());

    if (hitDistance > distanceInsideShape) return false;

    interaction.t = interactionA.t + hitDistance / rayLength;
    interaction.point = ray.at(interaction.t);

    interaction.normal = Vector3(1, 0, 0);  // arbitrary
    interaction.frontFace = true;           // also arbitrary
    interaction.materialPtr = phaseFunction;

    return true;
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    return shape->computeBoundingBox(t0, t1, outputBox);
  }
};

#endif