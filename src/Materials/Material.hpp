#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../Geometry/Vector3.hpp"
#include "../Geometry/Ray.hpp"
#include "../Core/SurfaceInteraction.hpp"

class Material {
 public:
  virtual bool scatter(const Ray& incoming, const SInteraction& interaction, Color& attenuation, Ray& scattered) const = 0;
  virtual Color emit(const UV& uv, const Point3F& point) const { return Color(0, 0, 0); }
};

#endif