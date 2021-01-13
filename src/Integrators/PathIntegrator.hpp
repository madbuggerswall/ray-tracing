#ifndef PATH_INTEGRATOR_HPP
#define PATH_INTEGRATOR_HPP

#include "Integrator.hpp"

class PathIntegrator : public Integrator {
 public:
  PathIntegrator(const Config config, const Scene& scene, const Camera& camera) : Integrator(config, scene, camera) {}

  Color tracePath(const Ray& ray, int bounceLimit) const override {
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
    return emission + attenuation * tracePath(scattered, bounceLimit - 1);
  }

  void render(Image& image) const override {
    Ray ray;
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << " " << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        Color pixelColor(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; ++s) {
          ray = camera.getRay(sampler.getRandomSample(i, j));
          pixelColor += tracePath(ray, bounceLimit);
        }
        image[(imageHeight - 1 - j) * imageWidth + i] = pixelColor;
      }
    }
  }
};
#endif