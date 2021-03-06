#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "../Math/ONB.hpp"
#include "../Math/Random.hpp"
#include "../Textures/SolidColor.hpp"
#include "Material.hpp"

class Lambertian : public Material {
 private:
  std::shared_ptr<Texture> albedo;

 public:
  Lambertian(const Color& albedo) : albedo(std::make_shared<SolidColor>(albedo)) {}
  Lambertian(std::shared_ptr<Texture> albedo) : albedo(albedo) {}

  bool scatter(const Ray& in, const SInteraction& interaction, Color& attenuation, Ray& scattered) const override {
    ONB orthonormalBasis(interaction.normal);
    Vector3 scatterDirection = orthonormalBasis.local(Random::cosineDirection());
    scattered = Ray(interaction.point, scatterDirection, in.getTime());
    attenuation = albedo->lookup(interaction.uv, interaction.point);
    return true;
  }

  bool scatter(const Ray& in, const SInteraction& interaction, Color& attenuation, Ray& scattered,
               Vector3 direction) const override {
    ONB orthonormalBasis(interaction.normal);
    Vector3 scatterDirection = orthonormalBasis.local(direction);
    scattered = Ray(interaction.point, scatterDirection, in.getTime());
    attenuation = albedo->lookup(interaction.uv, interaction.point);
    return true;
  }

  Color getColor(const UV& uv, const Point3& point) const override { return albedo->lookup(uv, point); }
  double brdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override { return 1.0 / Math::pi; }
  double pdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override {
    return std::abs(dot(wo, normal)) / Math::pi;
  }
};

#endif