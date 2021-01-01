#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include "../Core/stb_image_wrapper.hpp"
#include "Texture.hpp"

class ImageTexture : public Texture {
 private:
  const static int bytesPerPixel = 3;
  const float colorScale = 1.0 / 255.0;
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
    const float u = Math::clamp(uv.u, 0.0, 1.0);
    const float v = 1.0 - Math::clamp(uv.v, 0.0, 1.0);

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    auto pixel = dataPtr + j * bytesPerScanline + i * bytesPerPixel;
    return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);

    // auto pixelIndex = j * bytesPerScanline + i * bytesPerPixel;
    // return Color(colorScale * data[pixelIndex], colorScale * data[pixelIndex + 1], colorScale * data[pixelIndex +
    // 2]);
  }
};
#endif