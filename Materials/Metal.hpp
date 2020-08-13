#ifndef METAL_HPP
#define METAL_HPP

#include "../Ray.hpp"
#include "../Utilities.hpp"
#include "../Vector3.hpp"
#include "Material.hpp"

class Metal : public Material {
 private:
  Color albedo;
  double fuzz;

 public:
  Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(std::clamp(fuzz, 0.0, 1.0)) {}

  virtual bool scatter(const Ray& incoming, const HitRecord& hitRecord, Color& attenuation, Ray& scattered,
                       double& pdf) const override {
    Vector3 incomingDirection = incoming.getDirection().normalized();
    Vector3 reflected = incomingDirection.reflect(hitRecord.normal);
    scattered = Ray(hitRecord.point, reflected + fuzz * Random::vectorInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.getDirection(), hitRecord.normal) > 0);
  }
};

#endif