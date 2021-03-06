#ifndef METAL_HPP
#define METAL_HPP

#include "../Math/Math.hpp"
#include "../Math/Random.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Vector3.hpp"
#include "Material.hpp"

class Metal : public Material {
 private:
  Color albedo;
  double fuzz;

  // Dummy variable
  double glossiness = 25.0;

 public:
  Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(Math::clamp(fuzz, 0.0, 1.0)) {}

  virtual bool scatter(const Ray& incoming, const SInteraction& interaction, Color& attenuation,
                       Ray& scattered) const override {
    const Vector3 incomingDirection = incoming.direction.normalized();
    const Vector3 reflected = incomingDirection.reflect(interaction.normal);
    scattered = Ray(interaction.point, reflected + fuzz * Random::vectorInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.direction, interaction.normal) > 0);
  }

  double brdf(const Vector3& wi, const Vector3& n, const Vector3& wo) const override {
    const double won = dot(wo, n);
    const double win = dot(wi, n);
    const Vector3 reflected = (-wi).reflect(n);
    return (glossiness + 2.0) / (2.0 * Math::pi) * pow(std::fmax(dot(reflected, wo), 0.0), glossiness) /
           std::fmax(std::abs(win), std::abs(won));
  }

  double pdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override {
    const Vector3 reflected = (-wi).reflect(normal);
    return (glossiness + 1.0) / (2.0 * Math::pi) * std::pow(std::fmax(dot(reflected, wo), 0.0), glossiness);
  }
};

#endif