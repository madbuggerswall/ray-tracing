#ifndef PROBABILTY_DENSITY_FUNCTION_HPP
#define PROBABILTY_DENSITY_FUNCTION_HPP

#include "GeometricalObject.hpp"
#include "OrthonormalBasis.hpp"
#include "Utilities.hpp"

class ProbabilityDensityFunction;
using PDF = ProbabilityDensityFunction;

class ProbabilityDensityFunction {
 public:
  ~ProbabilityDensityFunction() {}

  virtual double value(const Vector3& direction) const = 0;
  virtual Vector3 generate() const = 0;
};

class CosinePDF : public ProbabilityDensityFunction {
 private:
  ONB onb;

 public:
  CosinePDF(const Vector3 vectorW) : onb(ONB(vectorW)) {}
  virtual double value(const Vector3& direction) const override {
    auto cosine = dot(direction.normalized(), onb.getW());
    return (cosine <= 0) ? 0 : cosine / Math::pi;
  }
  virtual Vector3 generate() const override { return onb.local(Random::cosineDirection()); }
};

class GeoObjectPDF : public ProbabilityDensityFunction {
 public:
  GeoObjectPDF(std::shared_ptr<GeoObject> object, const Point3& origin) : object(object), origin(origin) {}

  virtual double value(const Vector3& direction) const override { return object->pdfValue(origin, direction); }

  virtual Vector3 generate() const override { return object->random(origin); }

 public:
  Point3 origin;
  std::shared_ptr<GeoObject> object;
};

class MixturePDF : public ProbabilityDensityFunction {
 private:
 
 public:
  MixturePDF(){}
};

#endif