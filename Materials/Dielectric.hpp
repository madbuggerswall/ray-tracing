#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "../Utilities.hpp"
#include "Material.hpp"

class Dielectric : public Material {
 private:
  double refractiveIndex;
  double schlickApprox(double cosine, double refractiveRatio) const;

 public:
  Dielectric(double refractiveIndex) : refractiveIndex(refractiveIndex) {}

  virtual bool scatter(const Ray& incoming, const HitRecord& hitRecord, ScatterRecord& scatterRecotd) const {
    // attenuation = Color(1.0, 1.0, 1.0);
    double refractiveRatio;

    if (hitRecord.frontFace)
      refractiveRatio = 1.0 / refractiveIndex;
    else
      refractiveRatio = refractiveIndex;

    Vector3 direction = incoming.getDirection().normalized();

    double cosTheta = std::fmin(dot(-direction, hitRecord.normal), 1.0);
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    if (refractiveRatio * sinTheta > 1.0) {
      Vector3 reflected = direction.reflect(hitRecord.normal);
      // scattered = Ray(hitRecord.point, reflected);
      return true;
    }

    double reflectProb = schlickApprox(cosTheta, refractiveRatio);
    if (Random::fraction() < reflectProb) {
      Vector3 reflected = direction.reflect(hitRecord.normal);
      // scattered = Ray(hitRecord.point, reflected);
      return true;
    }

    Vector3 refracted = direction.refract(hitRecord.normal, refractiveRatio);
    // scattered = Ray(hitRecord.point, refracted);
    return true;
  }
};

double Dielectric::schlickApprox(double cosine, double refractiveRatio) const {
  // r0: reflection coefficient for light incoming parallel to the normal.
  auto r0 = (1 - refractiveRatio) / (1 + refractiveRatio);
  r0 *= r0;
  return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}
#endif