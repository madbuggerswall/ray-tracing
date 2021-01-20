#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <fstream>
#include <iostream>
#include <vector>

#include "../Math/Math.hpp"
#include "../Math/Vector3.hpp"

struct ImageInfo {
  size_t height;
  size_t width;
  size_t samplesPerPixel;
  size_t bounceLimit;
  size_t renderTime;
  std::string integratorName;

  friend std::ostream& operator<<(std::ostream& out, const ImageInfo& imageInfo) {
    return out << "# Image height: " << imageInfo.height << std::endl
               << "# Image width: " << imageInfo.width << std::endl
               << "# Samples per pixel: " << imageInfo.samplesPerPixel << std::endl
               << "# Ray bounce limit: " << imageInfo.bounceLimit << std::endl
               << "# Render time: " << imageInfo.renderTime << std::endl
               << "# Integrator: " << imageInfo.integratorName << std::endl;
  }
};

class Image {
 private:
  std::vector<Color> pixels;
  size_t height;
  size_t width;

 public:
  ImageInfo imageInfo;
  Image() = delete;
  Image(const size_t height, const size_t width) : height(height), width(width) {
    pixels = std::vector<Color>(height * width);
    imageInfo.height = height;
    imageInfo.width = width;
  }

  void writeToFile(std::string fileName, int samplesPerPixel);
  int toInt(double x) { return int(std::pow(1 - std::exp(-x), 1 / 2.2) * 255 + .5); }

  Color& operator[](int index) { return pixels[index]; }
  Color operator[](int index) const { return pixels[index]; }

  size_t getHeight() const { return height; }
  size_t getWidth() const { return width; }
};

void Image::writeToFile(std::string fileName, int samplesPerPixel) {
  if (fileName.length() > 0)
    fileName = fileName + ".ppm";
  else
    fileName = "output.ppm";

  std::ofstream outputFile(fileName);
  outputFile << "P3" << std::endl;
  outputFile << imageInfo << std::endl;
  outputFile << width << "	" << height << std::endl;
  outputFile << "255" << std::endl;

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      Color color = pixels[j * width + i];
      //	Divide the color total by the number of samples.
      //	Gamma-correct for gamma=2.0.
      // const auto scale = 1.0 / samplesPerPixel;
      const auto scale = 1.0 / double(samplesPerPixel);
      // Color printColor(std::sqrt(color.red * scale), std::sqrt(color.green * scale), std::sqrt(color.blue * scale));
      outputFile << static_cast<int>(toInt(color.red * scale)) << "	"
                 << static_cast<int>(toInt(color.green * scale)) << "	" << static_cast<int>(toInt(color.blue * scale))
                 << std::endl;
    }
  }
}

#endif