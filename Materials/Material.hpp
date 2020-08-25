#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../GeometricalObject.hpp"
#include "../ProbabilityDensityFunction.hpp"
#include "../Ray.hpp"
#include "../Vector3.hpp"

struct ScatterRecord {
  Ray specularRay;
  bool isSpecular;
  Color attenuation;
  std::shared_ptr<PDF> pdf;
};

class Material {
 private:
 public:
  virtual bool scatter(const Ray& in, const HitRecord& record, ScatterRecord& scatterRecord) const { return false; }
  virtual double scatterPDF(const Ray& incoming, const HitRecord& hitRecord, const Ray& scattered) const { return 0; }
  virtual Color emit(const Ray& in, const HitRecord& hitRecord, const UV& uv, const Point3& point) const {
    return Color(0, 0, 0);
  }
};

#endif