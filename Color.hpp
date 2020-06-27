#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "Vector3.hpp"

void writeColor(std::ostream& out, Color pixelColor) {
  out << static_cast<int>(255.999 * pixelColor.getX()) << "	"
      << static_cast<int>(255.999 * pixelColor.getY()) << "	"
      << static_cast<int>(255.999 * pixelColor.getZ()) << std::endl;
}

#endif