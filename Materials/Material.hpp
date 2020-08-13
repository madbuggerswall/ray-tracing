#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../GeometricalObject.hpp"
#include "../Ray.hpp"
#include "../Vector3.hpp"

class Material {
 private:
 public:
  virtual bool scatter(const Ray& in, const HitRecord& record, Color& albedo, Ray& scattered, double& pdf) const {
    return false;
  }
  virtual double scatterPDF(const Ray& incoming, const HitRecord& hitRecord, const Ray& scattered) const { return 0; }
  virtual Color emit(const Ray& in, const HitRecord& hitRecord, const UV& uv, const Point3& point) const {
    return Color(0, 0, 0);
  }
};

#endif