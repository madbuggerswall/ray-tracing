#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <fstream>
#include <iostream>
#include <vector>

#include "Geometry/Vector3.hpp"
#include "Math.hpp"

class Image {
 private:
  std::vector<Color> pixels;
  size_t height;
  size_t width;

 public:
  Image() = delete;
  Image(const size_t height, const size_t width) : height(height), width(width) {
    pixels = std::vector<Color>(height * width);
  }

  void writeToFile(std::string fileName, int samplesPerPixel);

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
  outputFile << width << "	" << height << std::endl;
  outputFile << "255" << std::endl;

  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      Color color = pixels[j * width + i];
      auto red = color.x;
      auto green = color.y;
      auto blue = color.z;

      //	Divide the color total by the number of samples.
      //	Gamma-correct for gamma=2.0.
      const auto scale = 1.0 / samplesPerPixel;
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