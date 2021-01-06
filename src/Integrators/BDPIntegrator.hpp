#ifndef BDP_PATH_INTEGRATOR_HPP
#define BDP_PATH_INTEGRATOR_HPP

#include "Integrator.hpp"
#include "Path.hpp"

// TODO: BPT
class BidirectionalPathIntegrator : public Integrator {
  std::vector<Path> camPaths;
  std::vector<Path> lightPaths;

  int minPathLength = 1;
  float lightArea;

 public:
  BidirectionalPathIntegrator(const CConfig& config, const Scene& scene, const Camera& camera) :
      Integrator(config, scene, camera) {
    lightArea = getTotalLightArea();
  }

  float getTotalLightArea() {
    float area = 0;
    for (auto light : scene.getLights()) { area += light->getArea(); }
    return area;
  }

  void tracePath(const Ray& ray, int bounceLimit, Path& path) const {
    if (bounceLimit <= 0) return;
    SurfaceInteraction interaction;

    // If a ray does not hit anything in the scene, return background color
    if (!scene.intersect(ray, 0.001, Math::infinity, interaction)) return;

    // set path data
    path.add(Vertex(interaction));

    Ray scattered;
    Color attenuation;
    if (interaction.materialPtr->scatter(ray, interaction, attenuation, scattered)) {
      tracePath(scattered, bounceLimit - 1, path);
    }
  }

  void render(Image& image) const override {
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "	" << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        Color pixelColor(0, 0, 0);
        for (int s = 0; s < samplesPerPixel; ++s) {
          Path camPath = generateCameraPath(i, j);
          Path lightPath = generateLightPath();
          PathContribution pathContribution = combinePaths(camPath, lightPath);

          pixelColor += pathContribution.accumulatePathContribution(pathContribution.scalarContrib);
        }
        image[j * imageWidth + i] = pixelColor;
      }
    }
  }

  Path generateCameraPath(ushort pixelX, ushort pixelY) const {
    Path path;
    Ray ray = camera.getRay(sampler.getRandomSample(pixelX, pixelY));
    path.add(Vertex(ray.origin, camera.getW()));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  Path generateLightPath() const {
    Path path;
    auto randomLight = scene.getRandomLight();
    auto lightMaterial = scene.getRandomLight()->getMaterial();
    Ray ray(randomLight->samplePoint(), Random::unitVector());
    path.add(Vertex(ray.origin, ray.direction, lightMaterial));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  bool isConnectable(const Path& camPath, const Path& lightPath) const {
    bool result;
    if (camPath.empty() && (lightPath.length() >= 2)) {
      // No direct hit to the film (pinhole)
      result = false;
    } else if ((camPath.length() >= 2) && lightPath.empty()) {
      // Direct hit to the light source
      bool isLastVertLight = dynamic_cast<DiffuseLight*>(&*(camPath.last().materialPtr));
      result = isLastVertLight;
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

  inline double geometryTerm(const Vertex& e0, const Vertex& e1) const {
    const Vector3 distVec = e1.point - e0.point;
    const double distSquared = distVec.magnitudeSquared();
    return std::abs(dot(e0.normal, distVec) * dot(e1.normal, distVec)) / (distSquared * distSquared);
  }

  inline double directionToArea(const Vertex& current, const Vertex& next) const {
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
        // bool isVertLight = std::is_same_v<decltype(path[i].materialPtr), std::shared_ptr<DiffuseLight>>;
        bool isVertLight = dynamic_cast<DiffuseLight*>(&*(path[i].materialPtr));
        if (isVertLight) {
          const Vector3 d0 = (path[i - 1].point - path[i].point).normalized();
          const double L = path[i].materialPtr->brdf(d0, path[i].normal, d0);
          color *= path[i].materialPtr->emit(path[i].interaction.uv, path[i].interaction.point) * L;
        } else {
          color *= 0.0;
        }
      } else {
        const Vector3 d0 = (path[i - 1].point - path[i].point).normalized();
        const Vector3 d1 = (path[i + 1].point - path[i].point).normalized();
        double BRDF = 0.0;
        if (path[i].materialPtr != nullptr) { BRDF = path[i].materialPtr->brdf(d0, path[i].normal, d1); }
        const UV uv = path[i].interaction.uv;
        const Point3 point = path[i].interaction.point;
        const Color materialColor = path[i].materialPtr->emit(uv, point);
        const float geometryTermVal = geometryTerm(path[i], path[i + 1]);
        color *= materialColor * BRDF * geometryTermVal;
      }
      if (color.maxComponent() == 0.0) return color;
    }
    return color;
  }

  PathContribution combinePaths(const Path& eyePath, const Path& lightPath) const {
    PathContribution result;

    // maxEvents = the maximum number of vertices
    for (int pathLength = minPathLength; pathLength <= bounceLimit; pathLength++) {
      for (int numEyeVertices = 0; numEyeVertices <= pathLength + 1; numEyeVertices++) {
        const int numLightVertices = (pathLength + 1) - numEyeVertices;

        if (numEyeVertices == 0) continue;  // no direct hit to the film (pinhole)
        if (numEyeVertices > eyePath.length()) continue;
        if (numLightVertices > lightPath.length()) continue;

        // extract subpaths
        Path eyeSubpath = eyePath;
        Path lightSubpath = lightPath;

        // check the path visibility
        double px = -1.0, py = -1.0;
        if (!isConnectable(eyeSubpath, lightSubpath)) continue;

        // construct a full path
        Path sampledPath;
        for (int i = 0; i < numEyeVertices; i++) sampledPath.add(eyePath[i]);
        for (int i = 0; i < numLightVertices; i++) sampledPath.add(lightPath[numLightVertices - i - 1]);

        // evaluate the path
        Color color = pathThroughput(sampledPath);
        double pathPDF = pathProbablityDensity(sampledPath, pathLength, numEyeVertices, numLightVertices);
        double weight = MISWeight(sampledPath, numEyeVertices, numLightVertices, pathLength);
        if ((weight <= 0.0) || (pathPDF <= 0.0)) continue;

        color *= (weight / pathPDF);
        if (color.maxComponent() <= 0.0) continue;

        // store the pixel contribution
        result.add(Contribution(color, px, py));

        // scalar contribution function
        result.scalarContrib = std::fmax(color.maxComponent(), result.scalarContrib);
      }
    }
    return result;
  }

  double MISWeight(const Path& sampledPath, const int numEyeVertices, const int numLightVertices,
                   const int pathLength) const {
    const double p_i = pathProbablityDensity(sampledPath, pathLength, numEyeVertices, numLightVertices);
    const double p_all = pathProbablityDensity(sampledPath, pathLength);
    if ((p_i == 0.0) || (p_all == 0.0)) {
      return 0.0;
    } else {
      return std::fmax(std::fmin(p_i / p_all, 1.0), 0.0);
    }
  }

  float lambertianPDF(const Vector3& wi, const Vector3& normal, const Vector3& wo) const {
    return std::abs(dot(wo, normal)) / Math::pi;
  }

  double pathProbablityDensity(const Path& sampledPath, const int pathLength, const int specifiedNumEyeVertices = -1,
                               const int specifiedNumLightVertices = -1) const {
    KahanAdder sumPDFs(0.0);
    bool specified = (specifiedNumEyeVertices != -1) && (specifiedNumLightVertices != -1);

    // number of eye subpath vertices
    for (int numEyeVertices = 0; numEyeVertices <= pathLength + 1; numEyeVertices++) {
      // extended BPT
      double pdfValue = 1.0;

      // number of light subpath vertices
      int numLightVertices = (pathLength + 1) - numEyeVertices;

      // we have pinhole camera
      if (numEyeVertices == 0) continue;

      // add all?
      if (specified && ((numEyeVertices != specifiedNumEyeVertices) || (numLightVertices != specifiedNumLightVertices)))
        continue;

      // sampling from the eye
      for (int i = -1; i <= numEyeVertices - 2; i++) {
        if (i == -1) {
          // PDF of sampling the camera position (the same delta function with the scaling 1.0 for all the PDFs - they
          // cancel out)
          pdfValue *= 1.0;
        } else if (i == 0) {
          pdfValue *= 1.0 / double(imageWidth * imageHeight);
          Vector3 Direction0 = (sampledPath[1].point - sampledPath[0].point).normalized();
          double cosTheta = dot(Direction0, camera.getW());
          double DistanceToScreen2 = camera.getDist(imageHeight) / cosTheta;
          DistanceToScreen2 = DistanceToScreen2 * DistanceToScreen2;
          pdfValue /= (cosTheta / DistanceToScreen2);

          pdfValue *= directionToArea(sampledPath[0], sampledPath[1]);
        } else {
          // PDF of sampling ith vertex
          Vector3 direction0 = (sampledPath[i - 1].point - sampledPath[i].point).normalized();
          Vector3 direction1 = (sampledPath[i + 1].point - sampledPath[i].point).normalized();

          if (sampledPath[i].materialPtr != nullptr) {
            pdfValue *= sampledPath[i].materialPtr->pdf(direction0, sampledPath[i].normal, direction1);
          }

          pdfValue *= directionToArea(sampledPath[i], sampledPath[i + 1]);
        }
      }

      if (pdfValue != 0.0) {
        // sampling from the light source
        for (int i = -1; i <= numLightVertices - 2; i++) {
          if (i == -1) {
            // PDF of sampling the light position (assume area-based sampling)
            pdfValue *= (1.0 / lightArea);
          } else if (i == 0) {
            Vector3 direction0 = (sampledPath[pathLength - 1].point - sampledPath[pathLength].point).normalized();
            pdfValue *= lambertianPDF(sampledPath[pathLength].normal, sampledPath[pathLength].normal, direction0);
            pdfValue *= directionToArea(sampledPath[pathLength], sampledPath[pathLength - 1]);
          } else {
            // PDF of sampling (PathLength - i)th vertex
            Vector3 dir0 = (sampledPath[pathLength - (i - 1)].point - sampledPath[pathLength - i].point).normalized();
            Vector3 dir1 = (sampledPath[pathLength - (i + 1)].point - sampledPath[pathLength - i].point).normalized();

            if (sampledPath[pathLength - i].materialPtr != nullptr)
              pdfValue *= sampledPath[pathLength - i].materialPtr->pdf(dir0, sampledPath[pathLength - i].normal, dir1);

            pdfValue *= directionToArea(sampledPath[pathLength - i], sampledPath[pathLength - (i + 1)]);
          }
        }
      }
      if (specified && (numEyeVertices == specifiedNumEyeVertices) && (numLightVertices == specifiedNumLightVertices))
        return pdfValue;

      // sum the probability density (use Kahan summation algorithm to reduce numerical issues)
      sumPDFs.add(pdfValue);
    }
    return sumPDFs.sum;
  }
};

using BDPTIntegrator = BidirectionalPathIntegrator;
#endif