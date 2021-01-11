#ifndef ISOTROPIC_HPP
#define ISOTROPIC_HPP

#include "../Math/Random.hpp"
#include "../Textures/SolidColor.hpp"
#include "Material.hpp"

class Isotropic : public Material {
 private:
  std::shared_ptr<Texture> albedo;

 public:
  Isotropic(Color color) : albedo(std::make_shared<SolidColor>(color)) {}
  Isotropic(std::shared_ptr<Texture> albedo) : albedo(albedo) {}

  virtual bool scatter(const Ray& in, const SInteraction& interaction, Color& attenuation,
                       Ray& scattered) const override {
    scattered = Ray(interaction.point, Random::vectorInUnitSphere(), in.getTime());
    attenuation = albedo->lookup(interaction.uv, interaction.point);
    return true;
  }

	// Lambertian BRDF
	float brdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override { return 1.0 / Math::pi; }
	
	// Lambertian BRDF
  float pdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override {
    return std::abs(dot(wo, normal)) / Math::pi;
  }
};

#endif