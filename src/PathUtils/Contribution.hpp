#ifndef CONTRIBUTION_HPP
#define CONTRIBUTION_HPP

#include "../Math/Vector3.hpp"

struct Contribution {
  Color color;
  float x, y;

  Contribution() : color(), x(0), y(0) {}
  Contribution(const Color& color, const int x, const int y) : color(color), x(x), y(y) {}

  // Copy constructor
  Contribution(const Contribution& other) : color(other.color), x(other.x), y(other.y) {}
  // Move constructor
  Contribution(Contribution&& other) noexcept :
      color(std::move(other.color)),
      x(std::exchange(other.x, 0)),
      y(std::exchange(other.y, 0)) {}
  // Copy assignment
  Contribution& operator=(const Contribution& other) {
    color = other.color;
    x = other.x;
    y = other.y;
    return *this;
  }
  // Move assignment
  Contribution& operator=(Contribution&& other) noexcept {
    color = std::move(other.color);
    x = std::move(other.x);
    y = std::move(other.y);
    return *this;
  }
};

#endif