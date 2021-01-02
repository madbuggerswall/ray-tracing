#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../Core/SurfaceInteraction.hpp"
#include "../Geometry/Ray.hpp"
#include "../Geometry/Vector3.hpp"

class Material {
 public:
  virtual bool scatter(const Ray& incoming, const SInteraction& interaction, Color& attenuation, Ray& scattered) const = 0;
  virtual Color emit(const UV& uv, const Point3& point) const { return Color(0, 0, 0); }
};

#endif