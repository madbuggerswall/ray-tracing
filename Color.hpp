#ifndef COLOR_HPP
#define COLOR_HPP

#include <fstream>
#include <iostream>
#include <vector>

#include "Geometry/Vector3.hpp"
#include "Math.hpp"

class Image {
 private:
  std::vector<std::vector<Color>> pixels;
  size_t height;
  size_t width;

 public:
  Image() = delete;
  Image(const size_t height, const size_t width) : height(height), width(width) {
    pixels = std::vector<std::vector<Color>>(height, std::vector<Color>(width));
  }

  std::vector<Color>& operator[](int index) { return pixels[index]; }
  std::vector<Color> operator[](int index) const { return pixels[index]; }

  size_t getHeight() const { return height; }
  size_t getWidth() const { return width; }
};

void writeColor(std::ostream& out, Color pixelColor, int samplesPerPixel) {
  auto red = pixelColor.x;
  auto green = pixelColor.y;
  auto blue = pixelColor.z;

  //	Divide the color total by the number of samples.
  //	Gamma-correct for gamma=2.0.
  auto scale = 1.0 / samplesPerPixel;
  red = std::sqrt(red * scale);
  green = std::sqrt(green * scale);
  blue = std::sqrt(blue * scale);

  out << static_cast<int>(256 * Math::clamp(red, 0.0, 0.999)) << "	"
      << static_cast<int>(256 * Math::clamp(green, 0.0, 0.999)) << "	"
      << static_cast<int>(256 * Math::clamp(blue, 0.0, 0.999)) << std::endl;
}

void writeImage(const Image& image, std::string fileName, int samplesPerPixel) {
  if (fileName.length() > 0)
    fileName = fileName + ".ppm";
  else
    fileName = "output.ppm";

  std::ofstream outputFile(fileName);

  auto imageHeight = image.getHeight();
  auto imageWidth = image.getWidth();

  outputFile << "P3" << std::endl;
  outputFile << imageWidth << "	" << imageHeight << std::endl;
  outputFile << "255" << std::endl;

  for (size_t i = 0; i < imageHeight; i++) {
    for (size_t j = 0; j < imageWidth; j++) {
      Color color = image[imageHeight - 1 - i][j];
      auto red = color.x;
      auto green = color.y;
      auto blue = color.z;

      //	Divide the color total by the number of samples.
      //	Gamma-correct for gamma=2.0.
      auto scale = 1.0 / samplesPerPixel;
      red = std::sqrt(red * scale);
      green = std::sqrt(green * scale);
      blue = std::sqrt(blue * scale);

      outputFile << static_cast<int>(256 * Math::clamp(red, 0.0, 0.999)) << "	"
                 << static_cast<int>(256 * Math::clamp(green, 0.0, 0.999)) << "	"
                 << static_cast<int>(256 * Math::clamp(blue, 0.0, 0.999)) << std::endl;
    }
  }
}

#endif