#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "../Geometry/Point2.hpp"
#include "Random.hpp"
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

  Sampler(const CConfig& config) : Sampler(config.imageHeight, config.imageWidth, config.samplesPerPixel) {}

  Point2 getRandomSample(ushort pixelX, ushort pixelY) const {
    const float u = (pixelX + Random::fraction()) / (imageWidth - 1);
    const float v = (pixelY + Random::fraction()) / (imageHeight - 1);
    return Point2(u, v);
  }

  Point2 getUniformSample(ushort pixelX, ushort pixelY, ushort index) const {
    const ushort i = index % samplesPerEdge;
    const ushort j = index / samplesPerEdge;
    const float offsetX = i / samplesPerEdge + 1 / (2 * samplesPerEdge);
    const float offsetY = j / samplesPerEdge + 1 / (2 * samplesPerEdge);

    const float u = (pixelX + offsetX) / (imageWidth - 1);
    const float v = (pixelY + offsetY) / (imageHeight - 1);
    return Point2(u, v);
  }

  Point2 getStratifiedSample(ushort pixelX, ushort pixelY, ushort index) const {
    const ushort i = index % samplesPerEdge;
    const ushort j = index / samplesPerEdge;
    const float offsetX = i / samplesPerEdge + 1 / (2 * samplesPerEdge) + (Random::fraction() - 0.5) / samplesPerEdge;
    const float offsetY = j / samplesPerEdge + 1 / (2 * samplesPerEdge) + (Random::fraction() - 0.5) / samplesPerEdge;
    const float u = (pixelX + offsetX) / (imageWidth - 1);
    const float v = (pixelY + offsetY) / (imageHeight - 1);
    return Point2(u, v);
  }
};
#endif