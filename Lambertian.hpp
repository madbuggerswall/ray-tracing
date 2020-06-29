#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "Material.hpp"
#include "Ray.hpp"
#include "Utilities.hpp"
#include "Vector3.hpp"

class Lambertian : public Material {
 private:
  Color albedo;

 public:
  Lambertian(const Color& albedo) : albedo(albedo) {}

  virtual bool scatter(const Ray& incoming, const HitRecord& HitRecord,
                       Color& attenuation, Ray& scattered) const override {
    Vector3 scatterDirection = HitRecord.normal + Random::unitVector();
    scattered = Ray(HitRecord.point, scatterDirection);
    attenuation = albedo;
    return true;
  }
};

#endif