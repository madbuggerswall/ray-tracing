#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "../Math/Point2.hpp"
#include "../Math/Random.hpp"
#include "Configuration.hpp"

class Sampler {
 private:
  ushort imageHeight;
  ushort imageWidth;
  ushort samplesPerPixel;

  ushort samplesPerEdge;

 public:
  Sampler(ushort imageHeight, ushort imageWidth, ushort samplesPerPixel) :
      imageHeight(imageHeight),
      imageWidth(imageWidth),
      samplesPerPixel(samplesPerPixel),
      samplesPerEdge(std::sqrt(samplesPerPixel)) {}

  Sampler(const Config& config) : Sampler(config.imageHeight, config.imageWidth, config.samplesPerPixel) {}

  Point2 getRandomSample(ushort pixelX, ushort pixelY) const {
    const double u = (pixelX + Random::fraction()) / (imageWidth - 1);
    const double v = (pixelY + Random::fraction()) / (imageHeight - 1);
    return Point2(u, v);
  }

  // MLT
  Point2 getRandomSample(ushort pixelX, ushort pixelY, double random1, double random2) const {
    const double u = (pixelX + random1) / (imageWidth - 1);
    const double v = (pixelY + random2) / (imageHeight - 1);
    return Point2(u, v);
  }

  Point2 getUniformSample(ushort pixelX, ushort pixelY, ushort index) const {
    const ushort i = index % samplesPerEdge;
    const ushort j = index / samplesPerEdge;
    const double offsetX = i / samplesPerEdge + 1 / (2 * samplesPerEdge);
    const double offsetY = j / samplesPerEdge + 1 / (2 * samplesPerEdge);

    const double u = (pixelX + offsetX) / (imageWidth - 1);
    const double v = (pixelY + offsetY) / (imageHeight - 1);
    return Point2(u, v);
  }

  Point2 getStratifiedSample(ushort pixelX, ushort pixelY, ushort index) const {
    const ushort i = index % samplesPerEdge;
    const ushort j = index / samplesPerEdge;
    const double offsetX = i / samplesPerEdge + 1 / (2 * samplesPerEdge) + (Random::fraction() - 0.5) / samplesPerEdge;
    const double offsetY = j / samplesPerEdge + 1 / (2 * samplesPerEdge) + (Random::fraction() - 0.5) / samplesPerEdge;
    const double u = (pixelX + offsetX) / (imageWidth - 1);
    const double v = (pixelY + offsetY) / (imageHeight - 1);
    return Point2(u, v);
  }
};
#endif