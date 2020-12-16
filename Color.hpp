#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "Geometry/Vector3.hpp"
#include "Math.hpp"

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

#endif