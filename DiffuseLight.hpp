#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

#include "SurfaceInteraction.hpp"
#include "Material.hpp"
#include "Texture.hpp"


class DiffuseLight : public Material {
 private:
  std::shared_ptr<Texture> emission;

 public:
  DiffuseLight(std::shared_ptr<Texture> emission) : emission(emission) {}
  DiffuseLight(Color color) : emission(std::make_shared<SolidColor>(color)) {}

  virtual bool scatter(const Ray& in, const SInteraction& interaction, Color& attenuation, Ray& scattered) const override {
    return false;
  }

  virtual Color emit(const UV& uv, const Point3F& point) const override { return emission->lookup(uv, point); }
};
#endif