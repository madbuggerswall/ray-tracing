#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include "../Math/Math.hpp"
#include "../Math/Point3.hpp"

class Texture {
 private:
 public:
  virtual Color lookup(const UV& coordinates, const Point3& point) const = 0;
};

#endif