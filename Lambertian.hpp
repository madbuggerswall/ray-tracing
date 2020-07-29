#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "Material.hpp"
#include "Utilities.hpp"

class Lambertian : public Material {
 private:
  Color albedo;

 public:
  Lambertian(const Color& albedo) : albedo(albedo) {}

  virtual bool scatter(const Ray& incoming,
                       const HitRecord& hitRecord,
                       Color& attenuation,
                       Ray& scattered) const override {
    Vector3 scatterDirection = hitRecord.normal + Random::unitVector();
    scattered = Ray(hitRecord.point, scatterDirection, incoming.getTime());
    attenuation = albedo;
    return true;
  }
};

#endif