#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "../OrthonormalBasis.hpp"
#include "../Texture.hpp"
#include "../Utilities.hpp"
#include "Material.hpp"

class Lambertian : public Material {
 private:
  std::shared_ptr<Texture> albedo;

 public:
  Lambertian(const Color& albedo) : albedo(std::make_shared<SolidColor>(albedo)) {}
  Lambertian(std::shared_ptr<Texture> albedo) : albedo(albedo) {}

  virtual bool scatter(const Ray& in, const HitRecord& hitRecord, ScatterRecord& scatterRecord) const override {
    scatterRecord.isSpecular = false;
    scatterRecord.attenuation = albedo->lookup(hitRecord.uv, hitRecord.point);
    scatterRecord.pdf = std::make_shared<CosinePDF>(hitRecord.normal);
    return true;
  }

  double scatterPDF(const Ray& incoming, const HitRecord& hitRecord, const Ray& scattered) const override {
    auto cosine = dot(hitRecord.normal, scattered.getDirection().normalized());
    return cosine < 0 ? 0 : cosine / Math::pi;
  }
};

#endif