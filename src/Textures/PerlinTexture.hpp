#ifndef PERLIN_TEXTURE_HPP
#define PERLIN_TEXTURE_HPP

#include "../Core/PerlinNoise.hpp"
#include "Texture.hpp"

class PerlinTexture : public Texture {
 private:
  PerlinNoise perlinNoise;
  float scale;

 public:
  PerlinTexture() {}
  PerlinTexture(float scale) : scale(scale) {}
  virtual Color lookup(const UV& uv, const Point3& point) const override {
    // return Color(1, 1, 1) * 0.5 * (1.0 + perlinNoise.noise(scale * point));
    // return Color(1, 1, 1)  * perlinNoise.turbulence(scale * point);
    return Color(1, 1, 1) * 0.5 * (1 + std::sin(scale * point.z + 10 * perlinNoise.turbulence(point)));
  }
};

#endif