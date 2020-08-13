#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

#include "../HitRecord.hpp"
#include "../Texture.hpp"
#include "../Utilities.hpp"
#include "Material.hpp"

class DiffuseLight : public Material {
 private:
  std::shared_ptr<Texture> emission;

 public:
  DiffuseLight(std::shared_ptr<Texture> emission) : emission(emission) {}
  DiffuseLight(Color color) : emission(std::make_shared<SolidColor>(color)) {}

  virtual bool scatter(const Ray& in, const HitRecord& hitRecord, Color& attenuation, Ray& scattered,
                       double& pdf) const override {
    return false;
  }

  virtual Color emit(const Ray& in, const HitRecord& hitRecord, const UV& uv, const Point3& point) const override {
		if (hitRecord.frontFace)
    return emission->lookup(uv, point);
    else
        return Color(0,0,0);
  }
};
#endif