#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

#include "../Core/SurfaceInteraction.hpp"
#include "../Textures/SolidColor.hpp"
#include "Material.hpp"

class DiffuseLight : public Material {
 private:
  std::shared_ptr<Texture> emission;

 public:
  DiffuseLight(std::shared_ptr<Texture> emission) : emission(emission) {}
  DiffuseLight(Color color) : emission(std::make_shared<SolidColor>(color)) {}

  virtual bool scatter(const Ray& in, const SInteraction& interaction, Color& attenuation,
                       Ray& scattered) const override {
    return false;
  }

  virtual Color emit(const UV& uv, const Point3& point) const override { return emission->lookup(uv, point); }
  
	// Lambertian BRDF
  float brdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override { return 1.0 / Math::pi; }

  // Lambertian BRDF
  float pdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const override {
    return std::abs(dot(wo, normal)) / Math::pi;
  }
};
#endif