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

  bool hitLight = false;

  Vertex(Point3 point, Vector3 normal, bool hitLight) : point(point), normal(normal), hitLight(hitLight) {}
  Vertex(const SInteraction& interaction, bool hitLight) : Vertex(interaction.point, interaction.normal, hitLight) {}
};

class Path {
 private:
  std::vector<Vertex> vertices;

 public:
  Color contribution;
  ushort pixelX, pixelY;

  Path() {}
  void add(const Vertex& vertex) { vertices.push_back(vertex); }

  Vertex& operator[](int index) { return vertices[index]; }
  Vertex operator[](int index) const { return vertices[index]; }

  Vertex last() const { return vertices.back(); }
  Vertex first() const { return vertices.front(); }
  int length() const { return vertices.size(); }
  bool empty() const { return vertices.empty(); }
};

// TODO: BPT
class BidirectionalPathIntegrator : public Integrator {
  std::vector<Path> camPaths;
  std::vector<Path> lightPaths;

 public:
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
    if (!interaction.materialPtr->scatter(ray, interaction, attenuation, scattered)) {
      // Add vertex to path
      path.add(Vertex(interaction, true));
      return emission;
    }
    // Add vertex to path
    path.add(Vertex(interaction, false));

    // If a ray hits any other material, scatter by spawning a new ray in a recursive way.
    return emission + attenuation * tracePath(scattered, bounceLimit - 1, path);
  }

  void render(Image& image) const override {
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "	" << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        for (int s = 0; s < samplesPerPixel; ++s) {
          Path camPath = generateCameraPath(i, j);
          Path lightPath = generateLightPath();
          if (isConnectable(camPath, lightPath))
            image[j * imageWidth + i] = camPath.contribution + lightPath.contribution;
        }
      }
    }
  }

  Path generateCameraPath(ushort pixelX, ushort pixelY) const {
    Path path;
    Ray ray = camera.getRay(sampler.getRandomSample(pixelX, pixelY));
    path.contribution = tracePath(ray, bounceLimit, path);
    return path;
  }

  Path generateLightPath() const {
    Path path;
    Ray ray(scene.getRandomLight()->samplePoint(), Random::unitVector());
    path.contribution = tracePath(ray, bounceLimit, path);
    return path;
  }

  bool isConnectable(const Path& camPath, const Path& lightPath) const {
    bool result;
    if (camPath.empty() && lightPath.empty()) {
      result = false;
    } else if (camPath.empty() && (lightPath.length() >= 2)) {
      // No direct hit to the film (pinhole)
      result = false;
    } else if ((camPath.length() >= 2) && lightPath.empty()) {
      // Direct hit to the light source
      result = camPath.last().hitLight;
    } else if ((camPath.length() == 1) && (lightPath.length() >= 1)) {
      // light tracing
      SInteraction interaction;
      Ray ray(camPath[0].point, (lightPath.last().point - camPath.first().point).normalized());
      double tMax = (lightPath.last().point - camPath[0].point).magnitude() / ray.direction.magnitude();
      result = !scene.intersect(ray, 0.001, tMax, interaction);
    } else {
      // shadow ray connection
      SInteraction interaction;
      Ray ray(camPath.last().point, (lightPath.last().point - camPath.last().point).normalized());
      double tMax = (lightPath.last().point - camPath.last().point).magnitude() / ray.direction.magnitude();
      result = !scene.intersect(ray, 0.001, tMax, interaction);
    }
    return result;
  }

  // BPT connections
  // s: Number of vertices in light subpath
  // t: Number of vertices in eye subpath
  // - limit the connection to a specific technique if s and t are provided
  // Path CombinePaths(const Path& eyePath, const Path& lightPath) {
  //   Path result;
  //   // maxEvents = the maximum number of vertices
  //   for (int pathLength = minPathLength; pathLength <= maxPathLength; pathLength++) {
  //     for (int numEyeVertices = 0; numEyeVertices <= pathLength + 1; numEyeVertices++) {
  //       const int numLightVertices = (pathLength + 1) - numEyeVertices;

  //       if (numEyeVertices == 0) continue;  // no direct hit to the film (pinhole)
  //       if (numEyeVertices > eyePath.length()) continue;
  //       if (numLightVertices > lightPath.length()) continue;

  //       // extract subpaths
  //       Path camSubPath = eyePath;
  //       Path lightSubPath = lightPath;

  //       // check the path visibility
  //       if (!isConnectable(camSubPath, lightSubPath)) continue;

  //       // construct a full path
  //       Path sampledPath;
  //       for (int i = 0; i < numEyeVertices; i++) sampledPath[i] = eyePath[i];
  //       for (int i = 0; i < numLightVertices; i++) sampledPath[pathLength - i] = lightPath[i];

  //       // evaluate the path
  //       Color color = pathThroughput(sampledPath);
  //       double pathPDF = PathProbablityDensity(sampledPath, pathLength, numEyeVertices, numLightVertices);
  //       double weight = MISWeight(sampledPath, numEyeVertices, numLightVertices, pathLength);
  //       if ((weight <= 0.0) || (pathPDF <= 0.0)) continue;

  //       color *= (weight / pathPDF);
  //       if (color.max() <= 0.0) continue;

  //       // store the pixel contribution
  //       result[result.contributionCount] = Contribution(px, py, color);
  //       result.contributionCount++;

  //       // scalar contribution function
  //       result.scalarContrib = std::fmax(color.max(), result.scalarContrib);
  //     }
  //   }
  //   return result;
  // }
};

using BDPTIntegrator = BidirectionalPathIntegrator;
#endif