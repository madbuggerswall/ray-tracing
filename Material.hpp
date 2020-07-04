#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "GeometricalObject.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"

class Material {
 private:
 public:
  virtual bool scatter(const Ray& incoming, const HitRecord& hitRecord,
                       Color& attenuation, Ray& scattered) const = 0;
};

#endif