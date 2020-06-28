#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "Utilities.hpp"

void writeColor(std::ostream& out, Color pixelColor, int samplesPerPixel) {
  auto red = pixelColor.getX();
  auto green = pixelColor.getY();
  auto blue = pixelColor.getZ();

  auto scale = 1.0 / samplesPerPixel;
  red *= scale;
  green *= scale;
  blue *= scale;

  out << static_cast<int>(256 * Util::clamp(red, 0.0, 0.999)) << "	"
      << static_cast<int>(256 * Util::clamp(green, 0.0, 0.999)) << "	"
      << static_cast<int>(256 * Util::clamp(blue, 0.0, 0.999)) << std::endl;
}

#endif