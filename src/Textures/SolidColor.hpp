#ifndef SOLID_COLOR_HPP
#define SOLID_COLOR_HPP

#include "Texture.hpp"

class SolidColor : public Texture {
 private:
  Color color;

 public:
  SolidColor() {}
  SolidColor(Color color) : color(color) {}
  SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

  virtual Color lookup(const UV& coordinates, const Point3& point) const override { return color; }
};

#endif