#ifndef CHECKER_TEXTURE_HPP
#define CHECKER_TEXTURE_HPP

#include "SolidColor.hpp"

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

  virtual Color lookup(const UV& uv, const Point3F& point) const override {
    auto checkerSize = 10;
    auto sines = std::sin(checkerSize * point.x);
    sines *= std::sin(checkerSize * point.y);
    sines *= std::sin(checkerSize * point.z);
    if (sines < 0)
      return odd->lookup(uv, point);
    else
      return even->lookup(uv, point);
  }
};
#endif