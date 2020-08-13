#ifndef PROBABILTY_DENSITY_FUNCTION_HPP
#define PROBABILTY_DENSITY_FUNCTION_HPP

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

#endif