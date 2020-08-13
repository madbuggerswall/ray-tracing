#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "GeometricalObject.hpp"
#include "Materials/Isotropic.hpp"
#include "Materials/Material.hpp"
#include "Texture.hpp"
#include "Utilities.hpp"

class ConstantMedium : public GeometricalObject {
 private:
  std::shared_ptr<GeometricalObject> shape;
  std::shared_ptr<Material> phaseFunction;
  double negInvDensity;

 public:
  ConstantMedium(std::shared_ptr<GeoObject> shape, double density, std::shared_ptr<Texture> albedo) :
      shape(shape),
      negInvDensity(-1 / density),
      phaseFunction(std::make_shared<Isotropic>(albedo)) {}
  ConstantMedium(std::shared_ptr<GeoObject> shape, double density, Color color) :
      shape(shape),
      negInvDensity(-1 / density),
      phaseFunction(std::make_shared<Isotropic>(color)) {}

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    HitRecord hitRecA, hitRecB;

    if (!shape->hit(ray, -Math::infinity, Math::infinity, hitRecA)) return false;

    if (!shape->hit(ray, hitRecA.t + 0.0001, Math::infinity, hitRecB)) return false;

    if (hitRecA.t < tMin) hitRecA.t = tMin;
    if (hitRecB.t > tMax) hitRecB.t = tMax;

    if (hitRecA.t >= hitRecB.t) return false;

    if (hitRecA.t < 0) hitRecA.t = 0;

    const auto rayLength = ray.getDirection().magnitude();
    const auto distanceInsideShape = (hitRecB.t - hitRecA.t) * rayLength;
    const auto hitDistance = negInvDensity * std::log(Random::fraction());

    if (hitDistance > distanceInsideShape) return false;

    hitRecord.t = hitRecA.t + hitDistance / rayLength;
    hitRecord.point = ray.at(hitRecord.t);

    hitRecord.normal = Vector3(1, 0, 0);  // arbitrary
    hitRecord.frontFace = true;           // also arbitrary
    hitRecord.materialPtr = phaseFunction;

    return true;
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    return shape->computeBoundingBox(t0, t1, outputBox);
  }
};

#endif