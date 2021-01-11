#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../Core/SurfaceInteraction.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Vector3.hpp"

class Material {
 public:
  virtual bool scatter(const Ray& incoming, const SInteraction& iaction, Color& attenuation, Ray& scattered) const = 0;
  virtual Color emit(const UV& uv, const Point3& point) const { return Color(0, 0, 0); }
  virtual float brdf(const Vector3& wi, const Vector3& normal, const Vector3& wo) const = 0;
  virtual float pdf(const Vector3& wi, const Vector3& n, const Vector3& wo) const = 0;
};

#endif