#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "Material.hpp"
#include "Random.hpp"

class Dielectric : public Material {
 private:
  float refractiveIndex;
  float schlickApprox(float cosine, float refractiveRatio) const;

 public:
  Dielectric(float refractiveIndex) : refractiveIndex(refractiveIndex) {}

  virtual bool scatter(const Ray& incoming, const HitRecord& hitRecord, Color& attenuation,
                       Ray& scattered) const override {
    attenuation = Color(1.0, 1.0, 1.0);
    float refractiveRatio;
    if (hitRecord.frontFace)
      refractiveRatio = 1.0 / refractiveIndex;
    else
      refractiveRatio = refractiveIndex;
    Vector3F direction = incoming.getDirection().normalized();

    float cosTheta = std::fmin(dot(-direction, hitRecord.normal), 1.0);
    float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    if (refractiveRatio * sinTheta > 1.0) {
      Vector3F reflected = direction.reflect(hitRecord.normal);
      scattered = Ray(hitRecord.point, reflected);
      return true;
    }

    float reflectProb = schlickApprox(cosTheta, refractiveRatio);
    if (Random::fraction() < reflectProb) {
      Vector3F reflected = direction.reflect(hitRecord.normal);
      scattered = Ray(hitRecord.point, reflected);
      return true;
    }

    Vector3F refracted = direction.refract(hitRecord.normal, refractiveRatio);
    scattered = Ray(hitRecord.point, refracted);
    return true;
  }
};

float Dielectric::schlickApprox(float cosine, float refractiveRatio) const {
  // r0: reflection coefficient for light incoming parallel to the normal.
  auto r0 = (1 - refractiveRatio) / (1 + refractiveRatio);
  r0 *= r0;
  return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}
#endif