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

  virtual bool scatter(const Ray& in, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const override {
    Vector3 incomingDirection = in.getDirection().normalized();
    Vector3 reflected = incomingDirection.reflect(hitRecord.normal);
    scatterRecord.specularRay = Ray(hitRecord.point, reflected + fuzz * Random::vectorInUnitSphere());
    scatterRecord.attenuation = albedo;
    scatterRecord.isSpecular = true;
    scatterRecord.pdf = 0;
    return true;
  }
};

#endif