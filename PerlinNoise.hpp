#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <vector>

#include "Utilities.hpp"

class PerlinNoise {
 private:
  static const int pointCount = 256;
  std::vector<Vector3> randomVectors;
  std::vector<int> permX;
  std::vector<int> permY;
  std::vector<int> permZ;

  static void permute(std::vector<int>& points, int n) {
    for (int i = n - 1; i > 0; i--) {
      int target = Random::rangeInt(0, i);
      std::swap(points[i], points[target]);
    }
  }

  static std::vector<int> generatePerm() {
    std::vector<int> points;

    points.reserve(pointCount);

    for (size_t i = 0; i < pointCount; ++i) { points[i] = i; }
    permute(points, pointCount);
    return points;
  }

  // c[2][2][2]: Corners of a cube.
  inline double trilinearInterpolation(Vector3 c[2][2][2], double u, double v, double w) const {
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 2; ++j)
        for (int k = 0; k < 2; ++k) {
          Vector3 weightVector(u - i, v - j, w - k);
          accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) *
                   dot(c[i][j][k], weightVector);
        }

    return accum;
  }

 public:
  PerlinNoise() {
    randomVectors.reserve(pointCount);
    for (size_t i = 0; i < pointCount; ++i) { randomVectors[i] = Random::vectorRange(-1, 1).normalized(); }

    permX = generatePerm();
    permY = generatePerm();
    permZ = generatePerm();
  }

  double noise(const Point3& point) const {
    auto u = point.getX() - std::floor(point.getX());
    auto v = point.getY() - std::floor(point.getY());
    auto w = point.getZ() - std::floor(point.getZ());

    int x = std::floor(point.getX());
    int y = std::floor(point.getY());
    int z = std::floor(point.getZ());
    Vector3 c[2][2][2];

    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++)
          c[i][j][k] = randomVectors[permX[(x + i) & 255] ^ permY[(y + j) & 255] ^ permZ[(z + k) & 255]];

    return trilinearInterpolation(c, u, v, w);
  }

  double turbulence(const Point3& point, int depth = 7) const {
    auto accum = 0.0;
    auto factorPoint = point;
    auto weight = 1.0;
    for (size_t i = 0; i < depth; ++i) {
      accum += weight + noise(factorPoint);
      weight *= 0.5;
      factorPoint *= 2;
    }
    return std::fabs(accum);
  }
};

#endif