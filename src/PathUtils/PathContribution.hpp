#ifndef PATH_CONTRIBUTION_HPP
#define PATH_CONTRIBUTION_HPP

#include "../Core/Image.hpp"
#include "../Geometry/Vector3.hpp"
#include "Contribution.hpp"

class PathContribution {
  std::vector<Contribution> contributions;

 public:
  float scalarContrib;

  PathContribution() = delete;
  PathContribution(uint maxEvents) : scalarContrib(0) { contributions.reserve(maxEvents * maxEvents); }

  // Copy constructor
  PathContribution(const PathContribution& other) :
      contributions(other.contributions),
      scalarContrib(other.scalarContrib) {}
  // Move constructor
  PathContribution(PathContribution&& other) noexcept :
      contributions(std::move(other.contributions)),
      scalarContrib(std::exchange(other.scalarContrib, 0)) {}

  // Copy assignment
  PathContribution& operator=(const PathContribution& other) {
    contributions = other.contributions;
    scalarContrib = other.scalarContrib;
    return *this;
  }
  // Move assignment
  PathContribution& operator=(PathContribution&& other) noexcept {
    contributions = std::move(other.contributions);
    scalarContrib = std::exchange(other.scalarContrib, 0);
    return *this;
  }

  Contribution& operator[](int index) { return contributions[index]; }
  Contribution operator[](int index) const { return contributions[index]; }

  void add(const Contribution& contrib) { contributions.push_back(contrib); }

  Color accumulatePathContribution(const float scale) {
    if (scalarContrib == 0) return Color(0, 0, 0);
    Color result;
    for (auto contribution : contributions) {
      // const int ix = int(contributions[i].x);
      // const int iy = int(contributions[i].y);
      const Color color = contribution.color * scale;
      result += color;
    }
    return result;
  }

  void accumulatePathContribution(const float scale, Image& image) {
    int imageWidth = image.getWidth();
    if (scalarContrib == 0) return;
    for (auto contribution : contributions) {
      const int ix = int(contribution.x);
      const int iy = int(contribution.y);
      const Color color = contribution.color * scale;
      image[iy * imageWidth + ix] += color;
    }
  }
};
#endif