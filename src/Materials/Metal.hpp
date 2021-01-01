#ifndef METAL_HPP
#define METAL_HPP

#include "../Core/Math.hpp"
#include "../Core/Random.hpp"
#include "../Geometry/Ray.hpp"
#include "../Geometry/Vector3.hpp"
#include "Material.hpp"

class Metal : public Material {
 private:
  Color albedo;
  float fuzz;

 public:
  Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(Math::clamp(fuzz, 0.0, 1.0)) {}

  virtual bool scatter(const Ray& incoming, const SInteraction& interaction, Color& attenuation,
                       Ray& scattered) const override {
    const Vector3 incomingDirection = incoming.direction.normalized();
    const Vector3 reflected = incomingDirection.reflect(interaction.normal);
    scattered = Ray(interaction.point, reflected + fuzz * Random::vectorInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.direction, interaction.normal) > 0);
  }
};

#endif