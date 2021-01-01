#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "../Core/Camera.hpp"
#include "../Core/Configuration.hpp"
#include "../Core/Image.hpp"
#include "../Core/Random.hpp"
#include "../Core/Sampler.hpp"
#include "../Core/Scene.hpp"
#include "../Core/SurfaceInteraction.hpp"
#include "../Geometry/Point2.hpp"
#include "../Geometry/Point3.hpp"
#include "../Geometry/Ray.hpp"
#include "../Geometry/Vector3.hpp"
#include "../Materials/Material.hpp"

class Integrator {
 protected:
  ushort imageHeight;
  ushort imageWidth;

  ushort samplesPerPixel;
  ushort bounceLimit;

  Color background;

 public:
  Integrator() = delete;
  Integrator(const CConfig& config) :
      imageHeight(config.imageHeight),
      imageWidth(config.imageWidth),
      samplesPerPixel(config.samplesPerPixel),
      bounceLimit(config.bounceLimit),
      background(config.background) {}

  virtual Color tracePath(const Ray& ray, const Scene& scene, int bounceLimit) const { return Color(0.5, 0.5, 0.5); };
  virtual void render(const Camera& camera, const Scene& scene, const Sampler& sampler, Image& image) const {};
};

class PathIntegrator : public Integrator {
 public:
  PathIntegrator(const CConfig config) : Integrator(config) {}

  Color tracePath(const Ray& ray, const Scene& scene, int bounceLimit) const override {
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
    return emission + attenuation * tracePath(scattered, scene, bounceLimit - 1);
  }

  void render(const Camera& camera, const Scene& scene, const Sampler& sampler, Image& image) const override {
    Ray ray;
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "	" << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        Color pixelColor(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; ++s) {
          ray = camera.getRay(sampler.getRandomSample(i, j));
          pixelColor += tracePath(ray, scene, bounceLimit);
        }
        image[j * imageWidth + i] = pixelColor;
      }
    }
  }
};

class Vertex {
  Point3 point;
  Vector3 normal;

 public:
  Vertex(Point3 point, Vector3 normal) : point(point), normal(normal) {}
  Vertex(const SInteraction& interaction) : Vertex(interaction.point, interaction.normal) {}
};

class Path {
  std::vector<Vertex> vertices;
	Color contribution;
 public:
  Path() {}
  void addVertex(const Vertex& vertex) { vertices.push_back(vertex); }
};

class BidirectionalPathIntegrator : public Integrator {
  BidirectionalPathIntegrator(const CConfig config) : Integrator(config) {}

  Color tracePath(const Ray& ray, const Scene& scene, int bounceLimit) const override {
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
    return emission + attenuation * tracePath(scattered, scene, bounceLimit - 1);
  }

  Path generateCameraPath() {
    Path result;
    return result;
  }
};

#endif