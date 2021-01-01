#ifndef ISOTROPIC_HPP
#define ISOTROPIC_HPP

#include "../Core/Random.hpp"
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
};

#endif