#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "Camera.hpp"
#include "Configuration.hpp"
#include "Geometry/Point2.hpp"
#include "Geometry/Point3.hpp"
#include "Geometry/Ray.hpp"
#include "Geometry/Vector3.hpp"
#include "Image.hpp"
#include "Material.hpp"
#include "Random.hpp"
#include "Scene.hpp"
#include "SurfaceInteraction.hpp"

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

  Point2F getRandomSample(ushort pixelX, ushort pixelY) const {
    const float u = (pixelX + Random::fraction()) / (imageWidth - 1);
    const float v = (pixelY + Random::fraction()) / (imageHeight - 1);
    return Point2F(u, v);
  }

  Point2F getUniformSample(ushort pixelX, ushort pixelY, ushort index) const {
    const ushort i = index % samplesPerEdge;
    const ushort j = index / samplesPerEdge;
    const float offsetX = i / samplesPerEdge + 1 / (2 * samplesPerEdge);
    const float offsetY = j / samplesPerEdge + 1 / (2 * samplesPerEdge);

    const float u = (pixelX + offsetX) / (imageWidth - 1);
    const float v = (pixelY + offsetY) / (imageHeight - 1);
    return Point2F(u, v);
  }

  Point2F getStratifiedSample(ushort pixelX, ushort pixelY, ushort index) const {
    const ushort i = index % samplesPerEdge;
    const ushort j = index / samplesPerEdge;
    const float offsetX = i / samplesPerEdge + 1 / (2 * samplesPerEdge) + (Random::fraction() - 0.5) / samplesPerEdge;
    const float offsetY = j / samplesPerEdge + 1 / (2 * samplesPerEdge) + (Random::fraction() - 0.5) / samplesPerEdge;
    const float u = (pixelX + offsetX) / (imageWidth - 1);
    const float v = (pixelY + offsetY) / (imageHeight - 1);
    return Point2F(u, v);
  }
};

class Integrator {
 private:
  ushort imageHeight;
  ushort imageWidth;

  ushort samplesPerPixel;
  ushort bounceLimit;

  Color background;

 public:
  Integrator(const CConfig& config) :
      imageHeight(config.imageHeight),
      imageWidth(config.imageWidth),
      samplesPerPixel(config.samplesPerPixel),
      bounceLimit(config.bounceLimit),
      background(config.background) {}

  Color tracePath(const Ray& ray, const Color& background, const Scene& scene, int bounceLimit) {
    SInteraction interaction;
    // If a ray exceeds the bounce limit, return black.
    if (bounceLimit <= 0) return Color(0, 0, 0);

    // If a ray does not hit anything in the scene, return background color
    if (!scene.intersect(ray, 0.001, Math::infinity, interaction)) return background;

    Ray scattered;
    Color attenuation;
    Color emission = interaction.materialPtr->emit(interaction.uv, interaction.point);

    // If a ray hits a light, return the light's emission color.
    if (!interaction.materialPtr->scatter(ray, interaction, attenuation, scattered)) return emission;

    // If a ray hits any other material, scatter by spawning a new ray in a recursive way.
    return emission + attenuation * tracePath(scattered, background, scene, bounceLimit - 1);
  }

  void render(const Camera& camera, const Scene& scene, const Sampler& sampler, Image& image) {
    Ray ray;
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "	" << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        Color pixelColor(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; ++s) {
          ray = camera.getRay(sampler.getRandomSample(i, j));
          pixelColor += tracePath(ray, background, scene, bounceLimit);
        }
        image[j * imageWidth + i] = pixelColor;
      }
    }
  }
};

#endif