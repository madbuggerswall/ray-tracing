#ifndef METAL_HPP
#define METAL_HPP

#include "Geometry/Vector3.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Random.hpp"
#include "Geometry/Ray.hpp"

class Metal : public Material {
 private:
  Color albedo;
  float fuzz;

 public:
  Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(Math::clamp(fuzz, 0.0, 1.0)) {}

  virtual bool scatter(const Ray& incoming, const HitRecord& hitRecord, Color& attenuation,
                       Ray& scattered) const override {
    Vector3F incomingDirection = incoming.direction.normalized();
    Vector3F reflected = incomingDirection.reflect(hitRecord.normal);
    scattered = Ray(hitRecord.point, reflected + fuzz * Random::vectorInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.direction, hitRecord.normal) > 0);
  }
};

#endif