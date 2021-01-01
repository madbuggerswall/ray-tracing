#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "../Textures/SolidColor.hpp"
#include "Material.hpp"

class Lambertian : public Material {
 private:
  std::shared_ptr<Texture> albedo;

 public:
  Lambertian(const Color& albedo) : albedo(std::make_shared<SolidColor>(albedo)) {}
  Lambertian(std::shared_ptr<Texture> albedo) : albedo(albedo) {}

  virtual bool scatter(const Ray& in, const SInteraction& interaction, Color& attenuation,
                       Ray& scattered) const override {
    const Vector3F scatterDirection = interaction.normal + Random::unitVector();
    scattered = Ray(interaction.point, scatterDirection, in.getTime());
    attenuation = albedo->lookup(interaction.uv, interaction.point);
    return true;
  }
};

#endif