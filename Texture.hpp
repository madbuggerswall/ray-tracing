#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "PerlinNoise.hpp"
#include "Utilities.hpp"

class Texture {
 private:
 public:
  virtual Color lookup(const UV& coordinates, const Point3& point) const = 0;
};

class SolidColor : public Texture {
 private:
  Color color;

 public:
  SolidColor() {}
  SolidColor(Color color) : color(color) {}
  SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

  virtual Color lookup(const UV& coordinates, const Point3& point) const override { return color; }
};

class CheckerTexture : public Texture {
 private:
  std::shared_ptr<Texture> even;
  std::shared_ptr<Texture> odd;

 public:
  CheckerTexture() {}
  CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) : even(even), odd(odd) {}
  CheckerTexture(Color colorA, Color colorB) :
      even(std::make_shared<SolidColor>(colorA)),
      odd(std::make_shared<SolidColor>(colorB)) {}

  virtual Color lookup(const UV& uv, const Point3& point) const override {
    auto checkerSize = 10;
    auto sines = std::sin(checkerSize * point.getX());
    sines *= std::sin(checkerSize * point.getY());
    sines *= std::sin(checkerSize * point.getZ());
    if (sines < 0)
      return odd->lookup(uv, point);
    else
      return even->lookup(uv, point);
  }
};

class PerlinTexture : public Texture {
 private:
  PerlinNoise perlinNoise;
  double scale;

 public:
  PerlinTexture() {}
  PerlinTexture(double scale) : scale(scale) {}
  virtual Color lookup(const UV& uv, const Point3& point) const override {
    return Color(1, 1, 1) * 0.5 * (1.0 + perlinNoise.noise(scale * point));
    // return Color(1, 1, 1)  * perlinNoise.turbulence(scale * point);
  }
};
#endif