#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include "External/stb_image_wrapper.hpp"
#include "Geometry/Point3.hpp"
#include "Math.hpp"
#include "PerlinNoise.hpp"

class Texture {
 private:
 public:
  virtual Color lookup(const UV& coordinates, const Point3F& point) const = 0;
};

class SolidColor : public Texture {
 private:
  Color color;

 public:
  SolidColor() {}
  SolidColor(Color color) : color(color) {}
  SolidColor(float red, float green, float blue) : SolidColor(Color(red, green, blue)) {}

  virtual Color lookup(const UV& coordinates, const Point3F& point) const override { return color; }
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

class PerlinTexture : public Texture {
 private:
  PerlinNoise perlinNoise;
  float scale;

 public:
  PerlinTexture() {}
  PerlinTexture(float scale) : scale(scale) {}
  virtual Color lookup(const UV& uv, const Point3F& point) const override {
    // return Color(1, 1, 1) * 0.5 * (1.0 + perlinNoise.noise(scale * point));
    // return Color(1, 1, 1)  * perlinNoise.turbulence(scale * point);
    return Color(1, 1, 1) * 0.5 * (1 + std::sin(scale * point.z + 10 * perlinNoise.turbulence(point)));
  }
};

class ImageTexture : public Texture {
 private:
  const static int bytesPerPixel = 3;
  int width;
  int height;
  int bytesPerScanline;
  // std::vector<unsigned char> data;
  unsigned char* dataPtr;

 public:
  ImageTexture() : dataPtr(nullptr), width(0), height(0), bytesPerScanline(0) {}
  ImageTexture(std::string fileName) {
    auto componentsPerPixel = bytesPerPixel;
    dataPtr = stbi_load(fileName.c_str(), &width, &height, &componentsPerPixel, componentsPerPixel);
    // data = std::vector<unsigned char>(dataPtr, dataPtr + sizeof(dataPtr) / sizeof(dataPtr[0]));

    if (!dataPtr) {
      std::cerr << "ERROR: Could not load texture image file '" << fileName << "'.\n";
      width = height = 0;
    }

    bytesPerScanline = bytesPerPixel * width;
  }

  ~ImageTexture() { delete dataPtr; }

  virtual Color lookup(const UV& uv, const Point3F& point) const override {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (dataPtr == nullptr) {
      return Color(1, 0, 0);
      std::cerr << "empty file" << std::endl;
    };

    // Clamp input texture coordinates to [0,1] x [1,0]
    float u = Math::clamp(uv.u, 0.0, 1.0);
    float v = 1.0 - Math::clamp(uv.v, 0.0, 1.0);

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    const auto colorScale = 1.0 / 255.0;
    auto pixel = dataPtr + j * bytesPerScanline + i * bytesPerPixel;
    return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);

    // auto pixelIndex = j * bytesPerScanline + i * bytesPerPixel;
    // return Color(colorScale * data[pixelIndex], colorScale * data[pixelIndex + 1], colorScale * data[pixelIndex +
    // 2]);
  }
};
#endif