#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "Material.hpp"
#include "Texture.hpp"
#include "Utilities.hpp"

class Lambertian : public Material {
 private:
  std::shared_ptr<Texture> albedo;

 public:
  Lambertian(const Color& albedo) : albedo(std::make_shared<SolidColor>(albedo)) {}
  Lambertian(std::shared_ptr<Texture> albedo) : albedo(albedo) {}

  virtual bool scatter(const Ray& in, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const override {
    Vector3 scatterDirection = hitRecord.normal + Random::unitVector();
    scattered = Ray(hitRecord.point, scatterDirection, in.getTime());
    attenuation = albedo->lookup(hitRecord.uv, hitRecord.point);
    return true;
  }
};

#endif