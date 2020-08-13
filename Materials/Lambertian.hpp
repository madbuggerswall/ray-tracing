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

  virtual bool scatter(const Ray& in, const HitRecord& hitRecord, Color& albedo, Ray& scattered,
                       double& pdf) const override {
    OrthonormalBasis uvw(hitRecord.normal);
    auto scatterDirection = uvw.local(Random::cosineDirection());
    scattered = Ray(hitRecord.point, scatterDirection, in.getTime());
    albedo = this->albedo->lookup(hitRecord.uv, hitRecord.point);
    pdf = dot(uvw.getW(), scattered.getDirection()) / Math::pi;
    return true;
  }

  double scatterPDF(const Ray& incoming, const HitRecord& hitRecord, const Ray& scattered) const override {
    auto cosine = dot(hitRecord.normal, scattered.getDirection().normalized());
    return cosine < 0 ? 0 : cosine / Math::pi;
  }
};

#endif