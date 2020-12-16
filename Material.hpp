#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "GeometricalObject.hpp"
#include "Geometry/Vector3.hpp"
#include "Geometry/Ray.hpp"

class Material {
 private:
 public:
  virtual bool scatter(const Ray& incoming, const HitRecord& hitRecord, Color& attenuation, Ray& scattered) const = 0;
  virtual Color emit(const UV& uv, const Point3F& point) const { return Color(0, 0, 0); }
};

#endif