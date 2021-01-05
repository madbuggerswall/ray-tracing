#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "../Core/Random.hpp"
#include "Material.hpp"

class Dielectric : public Material {
 private:
  float refractiveIndex;
  float schlickApprox(float cosine, float refractiveRatio) const;

 public:
  Dielectric(float refractiveIndex) : refractiveIndex(refractiveIndex) {}

  bool scatter(const Ray& incoming, const SInteraction& iaction, Color& attenuation, Ray& scattered) const override {
    attenuation = Color(1.0, 1.0, 1.0);

    float refractiveRatio;
    if (iaction.frontFace)
      refractiveRatio = 1.0 / refractiveIndex;
    else
      refractiveRatio = refractiveIndex;

    const Vector3 direction = incoming.direction.normalized();

    const float cosTheta = std::fmin(dot(-direction, iaction.normal), 1.0);
    const float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    if (refractiveRatio * sinTheta > 1.0) {
      const Vector3 reflected = direction.reflect(iaction.normal);
      scattered = Ray(iaction.point, reflected);
      return true;
    }

    const float reflectProb = schlickApprox(cosTheta, refractiveRatio);
    if (Random::fraction() < reflectProb) {
      const Vector3 reflected = direction.reflect(iaction.normal);
      scattered = Ray(iaction.point, reflected);
      return true;
    }

    const Vector3 refracted = direction.refract(iaction.normal, refractiveRatio);
    scattered = Ray(iaction.point, refracted);
    return true;
  }

	// Dummy BRDF and PDF. TODO
  virtual float brdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override { return 1.0; }
  virtual float pdf(const Vector3& wi, const Vector3& n, const Vector3& wo) const override { return 1.0; }
};

float Dielectric::schlickApprox(float cosine, float refractiveRatio) const {
  // r0: reflection coefficient for light incoming parallel to the normal.
  auto r0 = (1 - refractiveRatio) / (1 + refractiveRatio);
  r0 *= r0;
  return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}
#endif