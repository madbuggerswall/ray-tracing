#ifndef ISOTROPIC_HPP
#define ISOTROPIC_HPP

#include "Material.hpp"
#include "Texture.hpp"
#include "Utilities.hpp"

class Isotropic : public Material {
 private:
  std::shared_ptr<Texture> albedo;

 public:
  Isotropic(Color color) : albedo(std::make_shared<SolidColor>(color)) {}
  Isotropic(std::shared_ptr<Texture> albedo) : albedo(albedo) {}

  virtual bool scatter(const Ray& in, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override {
    scattered = Ray(hitRecord.point, Random::vectorInUnitSphere(), in.getTime());
    attenuation = albedo->lookup(hitRecord.uv, hitRecord.point);
    return true;
  }
};

#endif