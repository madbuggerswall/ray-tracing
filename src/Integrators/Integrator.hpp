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
  Sampler sampler;
  Scene scene;
  Camera camera;

  Color background;

  ushort imageHeight;
  ushort imageWidth;

  ushort samplesPerPixel;
  ushort bounceLimit;

 public:
  Integrator() = delete;
  Integrator(const CConfig& config, const Scene& scene, const Camera& camera) :
      imageHeight(config.imageHeight),
      imageWidth(config.imageWidth),
      samplesPerPixel(config.samplesPerPixel),
      bounceLimit(config.bounceLimit),
      background(config.background),
      sampler(config),
      scene(scene),
      camera(camera) {}

  virtual Color tracePath(const Ray& ray, int bounceLimit) const { return Color(0.5, 0.5, 0.5); };
  virtual void render(Image& image) const {};
};

class PathIntegrator : public Integrator {
 public:
  PathIntegrator(const CConfig config, const Scene& scene, const Camera& camera) : Integrator(config, scene, camera) {}

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
      std::cout << "\rScanlines remaining: " << j << "	" << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        Color pixelColor(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; ++s) {
          ray = camera.getRay(sampler.getRandomSample(i, j));
          pixelColor += tracePath(ray, bounceLimit);
        }
        image[j * imageWidth + i] = pixelColor;
      }
    }
  }
};

class Vertex {
 public:
  Point3 point;
  Vector3 normal;

  Vertex(Point3 point, Vector3 normal) : point(point), normal(normal) {}
  Vertex(const SInteraction& interaction) : Vertex(interaction.point, interaction.normal) {}
};

class Path {
  std::vector<Vertex> vertices;

 public:
  Color contribution;
  ushort pixelX, pixelY;

  Path() {}
  void add(const Vertex& vertex) { vertices.push_back(vertex); }
};

// TODO: BPT
class BidirectionalPathIntegrator : public Integrator {
  std::vector<Path> camPaths;
  std::vector<Path> lightPaths;

  BidirectionalPathIntegrator(const CConfig& config, const Scene& scene, const Camera& camera) :
      Integrator(config, scene, camera) {}

  Color tracePath(const Ray& ray, int bounceLimit, Path& path) const {
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

    // Add vertex to path
    path.add(Vertex(interaction));

    // If a ray hits any other material, scatter by spawning a new ray in a recursive way.
    return emission + attenuation * tracePath(scattered, bounceLimit - 1, path);
  }

  void render(Image& image) const override {
    Ray ray;
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "	" << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        for (int s = 0; s < samplesPerPixel; ++s) { Path camPath = generateCameraPath(i, j); }
      }
    }
  }

  Path generateCameraPath(ushort pixelX, ushort pixelY) const {
    Path path;
    Ray ray = camera.getRay(sampler.getRandomSample(pixelX, pixelY));
    path.contribution = tracePath(ray, bounceLimit, path);
    return path;
  }
};

using BDPTIntegrator = BidirectionalPathIntegrator;
#endif