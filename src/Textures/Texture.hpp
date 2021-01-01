#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include "../Core/Math.hpp"
#include "../Geometry/Point3.hpp"

class Texture {
 private:
 public:
  virtual Color lookup(const UV& coordinates, const Point3F& point) const = 0;
};

#endif