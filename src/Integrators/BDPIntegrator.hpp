#ifndef BDP_PATH_INTEGRATOR_HPP
#define BDP_PATH_INTEGRATOR_HPP

#include "Integrator.hpp"

class Vertex {
 public:
  Point3 point;
  Vector3 normal;
  Color color;
  std::shared_ptr<Material> materialPtr;
  bool hitLight = false;

  Vertex(const Point3& point, const Vector3& normal, const bool hitLight) :
      point(point),
      normal(normal),
      color(0, 0, 0),
      hitLight(hitLight) {}

  Vertex(const Point3& point, const Vector3& normal, const Color& color, const bool hitLight) :
      point(point),
      normal(normal),
      color(color),
      hitLight(hitLight) {}

  Vertex(const Point3& point, const Vector3& normal, const Color& color, const bool hitLight,
         const std::shared_ptr<Material> materialPtr) :
      point(point),
      normal(normal),
      color(color),
      hitLight(hitLight),
      materialPtr(materialPtr) {}

  Vertex(const SInteraction& interaction, const Color color, const bool hitLight) :
      Vertex(interaction.point, interaction.normal, color, hitLight, interaction.materialPtr) {}
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
  void removeLast() { vertices.pop_back(); }
  void append(const Path& path) { vertices.insert(vertices.end(), path.vertices.begin(), path.vertices.end()); }
  void reverse() { std::reverse(vertices.begin(), vertices.end()); }
};

// TODO: BPT
class BidirectionalPathIntegrator : public Integrator {
  std::vector<Path> camPaths;
  std::vector<Path> lightPaths;

 public:
  BidirectionalPathIntegrator(const CConfig& config, const Scene& scene, const Camera& camera) :
      Integrator(config, scene, camera) {}

  void tracePath(const Ray& ray, int bounceLimit, Path& path, bool isLight) const {
    SInteraction interaction;
    // If a ray exceeds the bounce limit, return black.
    if (bounceLimit <= 0) return;

    // If a ray does not hit anything in the scene, return background color
    if (!scene.intersect(ray, 0.001, Math::infinity, interaction)) return;

    Ray scattered;
    Color attenuation;
    Color emission = interaction.materialPtr->emit(interaction.uv, interaction.point);

    // If a ray hits a light, return the light's emission color.
    if (!interaction.materialPtr->scatter(ray, interaction, attenuation, scattered) && !isLight) {
      // Add vertex to path
      path.add(Vertex(interaction, emission, true));
      tracePath(scattered, bounceLimit - 1, path, isLight);
    }
    // Add vertex to path
    path.add(Vertex(interaction, attenuation, false));

    tracePath(scattered, bounceLimit - 1, path, isLight);
  }

  Color evaluatePath(const Path& path, int index) const {
    SInteraction interaction;

    // If a ray exceeds the bounce limit, return black.
    if (!(index < path.length() - 1)) return Color(0, 0, 0);

    Vector3 direction = (path[index + 1].point - path[index].point).normalized();
    Ray ray(path[index].point, direction);
    // If a ray does not hit anything in the scene, return background color
    if (!scene.intersect(ray, 0.001, Math::infinity, interaction)) return background;

    Ray scattered;
    Color attenuation;
    Color emission = interaction.materialPtr->emit(interaction.uv, interaction.point);

    // If a ray hits a light, return the light's emission color.
    if (!interaction.materialPtr->scatter(ray, interaction, attenuation, scattered)) return emission;

    // If a ray hits any other material, scatter by spawning a new ray in a recursive way.
    return emission + attenuation * evaluatePath(path, index + 1);
  }

  void render(Image& image) const override {
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "	" << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        Color pixelColor(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; ++s) {
          Path camPath = generateCameraPath(i, j);
          Path lightPath;
          Path combined;
          combined.append(camPath);
          if (!camPath.last().hitLight) {
            lightPath = generateLightPath();
            if (isConnectable(camPath, lightPath)) {
              lightPath.reverse();
              combined.append(lightPath);
            }
          }
          pixelColor += evaluatePath(combined, 0);
        }
        image[j * imageWidth + i] = pixelColor;
      }
    }
  }

  Path generateCameraPath(ushort pixelX, ushort pixelY) const {
    Path path;
    Ray ray = camera.getRay(sampler.getRandomSample(pixelX, pixelY));
    path.add(Vertex(ray.origin, camera.getW(), false));
    tracePath(ray, bounceLimit / 2, path, false);
    return path;
  }

  Path generateLightPath() const {
    Path path;
    auto randomLight = scene.getRandomLight();
    auto lightMaterial = scene.getRandomLight()->getMaterial();
    Ray ray(randomLight->samplePoint(), Random::unitVector());
    path.add(Vertex(ray.origin, ray.direction, lightMaterial->emit(UV(), Point3()), true, lightMaterial));
    tracePath(ray, bounceLimit / 2, path, true);
    return path;
  }

  bool isConnectable(const Path& camPath, const Path& lightPath) const {
    bool result;
    if (camPath.empty() && (lightPath.length() >= 2)) {
      // No direct hit to the film (pinhole)
      result = false;
    } else if ((camPath.length() >= 2) && lightPath.empty()) {
      // Direct hit to the light source
      result = camPath.last().hitLight;
    } else if ((camPath.length() == 1) && (lightPath.length() >= 1)) {
      // light tracing
      SInteraction interaction;
      Ray ray(camPath.first().point, (lightPath.last().point - camPath.first().point).normalized());
      double tMax = (lightPath.last().point - camPath.first().point).magnitude() / ray.direction.magnitude();
      result = !scene.intersect(ray, 0.001, tMax - 0.001, interaction);
    } else {
      // shadow ray connection
      SInteraction interaction;
      Ray ray(camPath.last().point, (lightPath.last().point - camPath.last().point).normalized());
      double tMax = (lightPath.last().point - camPath.last().point).magnitude() / ray.direction.magnitude();
      result = !scene.intersect(ray, 0.001, tMax - 0.001, interaction);
    }
    return result;
  }

  inline double GeometryTerm(const Vertex& e0, const Vertex& e1) const {
    const Vector3 distVec = e1.point - e0.point;
    const double distSquared = distVec.magnitudeSquared();
    return std::abs(dot(e0.normal, distVec) * dot(e1.normal, distVec)) / (distSquared * distSquared);
  }

  inline double DirectionToArea(const Vertex& current, const Vertex& next) {
    const Vector3 dv = next.point - current.point;
    const double d2 = dv.magnitudeSquared();
    return std::abs(dot(next.normal, dv)) / (d2 * sqrt(d2));
  }

  Color pathThroughput(const Path& path) const {
    Color color = Color(1.0, 1.0, 1.0);
    for (int i = 0; i < path.length(); i++) {
      if (i == 0) {
        double W = 1.0 / double(imageWidth * imageHeight);
        Vector3 d0 = path[1].point - path[0].point;
        const double dist2 = d0.magnitudeSquared();
        d0 = d0 * (1.0 / sqrt(dist2));
        const double c = dot(d0, camera.getW());
        const double ds2 = (camera.getDist(imageHeight) / c) * (camera.getDist(imageHeight) / c);
        W /= (c / ds2);
        color *= (W * std::abs(dot(d0, path[1].normal) / dist2));
      } else if (i == (path.length() - 1)) {
        if (path[i].hitLight) {
          const Vector3 d0 = (path[i - 1].point - path[i].point).normalized();
          const double L = path[i].materialPtr->brdf(d0, path[i].normal, d0);
          color *= path[i].color * L;
        } else {
          color *= 0.0;
        }
      } else {
        const Vector3 d0 = (path[i - 1].point - path[i].point).normalized();
        const Vector3 d1 = (path[i + 1].point - path[i].point).normalized();
        double BRDF = 0.0;
        if (path[i].materialPtr != nullptr) { BRDF = path[i].materialPtr->brdf(d0, path[i].normal, d1); }
        color *= path[i].color * BRDF * GeometryTerm(path[i], path[i + 1]);
      }
      if (color.maxComponent() == 0.0) return color;
    }
    return color;
  }
};

using BDPTIntegrator = BidirectionalPathIntegrator;
#endif