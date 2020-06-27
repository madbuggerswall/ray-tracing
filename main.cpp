#include <iostream>

#include "Color.hpp"
#include "Vector3.hpp"

int main(int argc, char const* argv[]) {
  const int imageWidth = 256;
  const int imageHeight = 256;

  std::cout << "P3" << std::endl;
  std::cout << imageWidth << "	" << imageHeight << std::endl;
  std::cout << "255" << std::endl;

  for (int i = imageHeight - 1; i >= 0; --i) {
    std::cerr << "\rScanlines remaining: " << i << "	" << std::flush;
    for (int j = 0; j < imageWidth; ++j) {
      Color pixelColor(double(j) / (imageWidth - 1),
                       double(i) / (imageHeight - 1), 0.25);
      writeColor(std::cout, pixelColor);
    }
  }
  std::cerr << std::endl << "Done." << std::endl;

  return 0;
}
