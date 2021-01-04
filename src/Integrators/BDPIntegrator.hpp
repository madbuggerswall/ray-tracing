#ifndef BDP_PATH_INTEGRATOR_HPP
#define BDP_PATH_INTEGRATOR_HPP

#include "Integrator.hpp"

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
  void removeLast() { vertices.pop_back(); }
  void append(const Path& path) { vertices.insert(vertices.end(), path.vertices.begin(), path.vertices.end()); }
};

// TODO: BPT
class BidirectionalPathIntegrator : public Integrator {
  std::vector<Path> camPaths;
  std::vector<Path> lightPaths;

 public:
  BidirectionalPathIntegrator(const CConfig& config, const Scene& scene, const Camera& camera) :
      Integrator(config, scene, camera) {}

  void tracePath(const Ray& ray, int bounceLimit, Path& path) const {
    SInteraction interaction;
    // If a ray exceeds the bounce limit, return black.
    if (bounceLimit <= 0) return;

    // If a ray does not hit anything in the scene, return background color
    if (!scene.intersect(ray, 0.001, Math::infinity, interaction)) return;

    Ray scattered;
    Color attenuation;
    Color emission = interaction.materialPtr->emit(interaction.uv, interaction.point);

    // If a ray hits a light, return the light's emission color.
    if (!interaction.materialPtr->scatter(ray, interaction, attenuation, scattered)) {
      // Add vertex to path
      path.add(Vertex(interaction, true));
      return;
    }
    // Add vertex to path
    path.add(Vertex(interaction, false));

    tracePath(scattered, bounceLimit - 1, path);
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
            if (isConnectable(camPath, lightPath)) { combined.append(lightPath); }
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
    path.add(Vertex(ray.origin, Vector3(0, 0, 0), false));
    tracePath(ray, bounceLimit / 2, path);
    return path;
  }

  Path generateLightPath() const {
    Path path;
    Ray ray(scene.getRandomLight()->samplePoint(), Random::unitVector());
    path.add(Vertex(ray.origin, ray.direction, true));
    tracePath(ray, bounceLimit / 2, path);
    if (path.length() >= 2) {
      if (path.last().hitLight) { path.removeLast(); }
    }
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

//   Color pathThroughput(const Path& path) {
//     Color color = Color(1.0, 1.0, 1.0);
//     for (int i = 0; i < path.length(); i++) {
//       if (i == 0) {
//         double W = 1.0 / double(imageWidth * imageHeight);
//         Vector3 d0 = path[1].point - path[0].point;
//         const double dist2 = d0.magnitudeSquared();
//         d0 = d0 * (1.0 / sqrt(dist2));
//         const double c = dot(d0, camera.w);
//         const double ds2 = (camera.dist / c) * (camera.dist / c);
//         W /= (c / ds2);
//         color *= (W * std::abs(dot(d0, path[1].normal) / dist2));
//       } else if (i == (path.length() - 1)) {
//         if (path[i].hitLight) {
//           const Vector3 d0 = (path[i - 1].point - path[i].point).normalized();
//           const double L = LambertianBRDF(d0, path[i].normal, d0);
//           color *= sph[path[i].id].color * L;
//         } else {
//           color *= 0.0;
//         }
//       } else {
//         const Vector3 d0 = (path[i - 1].point - path[i].point).normalized();
//         const Vector3 d1 = (path[i + 1].point - path[i].point).normalized();
//         double BRDF = 0.0;
//         if (sph[path[i].id].refl == DIFF) {
//           BRDF = LambertianBRDF(d0, path[i].normal, d1);
//         } else if (sph[path[i].id].refl == GLOS) {
//           BRDF = GlossyBRDF(d0, path[i].normal, d1);
//         }
//         color *= sph[path[i].id].color * BRDF * GeometryTerm(path[i], path[i + 1]);
//       }
//       if (color.max() == 0.0) return color;
//     }
//     return color;
//   }
// };

using BDPTIntegrator = BidirectionalPathIntegrator;
#endif