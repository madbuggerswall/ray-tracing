#ifndef BDP_PATH_INTEGRATOR_HPP
#define BDP_PATH_INTEGRATOR_HPP

#include "../Core/Stopwatch.hpp"
#include "../PathUtils/Path.hpp"
#include "../PathUtils/PathContribution.hpp"
#include "Integrator.hpp"

// TODO: Scale the accumulate with b / scalarContrib.
class BidirectionalPathIntegrator : public Integrator {
 protected:
  //  Avoid direct light
  int minPathLength = 3;

  // bounces + 1 sampled point from camera or light.
  int maxEvents;

  double lightArea;
  double normConstant;

 public:
  BidirectionalPathIntegrator(const Config& config, const Scene& scene, const Camera& camera) :
      Integrator(config, scene, camera) {
    lightArea = getTotalLightArea();
    maxEvents = bounceLimit + 1;
    normConstant = estimateNormalizationConstant(10000);
  }

  double estimateNormalizationConstant(uint sampleCount) {
    std::cout << "Estimating normalization constant" << std::endl;
    Stopwatch stopwatch;
    stopwatch.start();
    double b = 0.0;
    for (int i = 0; i < sampleCount; i++) {
      b += combinePaths(generateCameraPath(), generateLightPath()).scalarContrib;
    }
    b /= double(sampleCount);
    stopwatch.stop();
    std::cout << "Normalization constant estimated: " << b << std::endl;
    stopwatch.printTime();
    return b;
  }

  double getTotalLightArea() {
    double area = 0;
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
		
    bool hitLight = dynamic_cast<DiffuseLight*>(&*(interaction.materialPtr));
    if (hitLight) return;

    Ray scattered;
    Color attenuation;
    if (interaction.materialPtr->scatter(ray, interaction, attenuation, scattered)) {
      tracePath(scattered, bounceLimit - 1, path);
    }
  }

  void render(Image& image) const override {
    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "  " << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        for (int s = 0; s < samplesPerPixel; ++s) {
          combinePaths(generateCameraPath(i, j), generateLightPath()).accumulatePathContribution(1.0, image);
        }
      }
    }
  }

  Path generateCameraPath() const {
    Path path(maxEvents);
    Ray ray = camera.getSample(imageHeight, imageWidth);
    path.add(Vertex(ray.origin, camera.getW()));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  Path generateCameraPath(ushort pixelX, ushort pixelY) const {
    Path path(maxEvents);
    Ray ray = camera.getRay(sampler.getRandomSample(pixelX, pixelY));
    path.add(Vertex(ray.origin, camera.getW()));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  Path generateLightPath() const {
    Path path(maxEvents);
    auto randomLight = scene.getRandomLight();
    auto lightMaterial = scene.getRandomLight()->getMaterial();
    Ray ray = randomLight->sampleDirection();
    path.add(Vertex(ray.origin, ray.direction, lightMaterial));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  bool isConnectable(const Path& camPath, const Path& lightPath, double& px, double& py) const {
    Vector3 direction;
    bool result;

    // Dummy
    SInteraction interaction;

    if (camPath.empty() && (lightPath.length() >= 2)) {
      // No direct hit to the film (pinhole)
      result = false;
    } else if ((camPath.length() >= 2) && lightPath.empty()) {
      // Direct hit to the light source
      direction = (camPath[1].point - camPath[0].point).normalized();
      bool isLastVertLight = dynamic_cast<DiffuseLight*>(&*(camPath.last().materialPtr));
      result = isLastVertLight;

    } else if ((camPath.length() == 1) && (lightPath.length() >= 1)) {
      // light tracing
      Ray ray(camPath.first().point, (lightPath.last().point - camPath.first().point).normalized());
      double tMax = (lightPath.last().point - camPath.first().point).magnitude() / ray.direction.magnitude();
      direction = ray.direction;
      result = !scene.intersect(ray, 0, tMax, interaction);
    } else {
      // shadow ray connection
      Ray ray(camPath.last().point, (lightPath.last().point - camPath.last().point).normalized());
      double tMax = (lightPath.last().point - camPath.last().point).magnitude() / ray.direction.magnitude();
      direction = (camPath[1].point - camPath[0].point).normalized();

      result = !scene.intersect(ray, 0, tMax, interaction);
    }
    if (!result) return result;
    // get the pixel location
    Point3 origin = camera.getOrigin();
    Vector3 w = camera.getW();
    double dist = camera.getDist(imageHeight);
    Point3 screenCenter = origin + (w * dist);
    Vector3 screenPosition = origin + (direction * (dist / dot(direction, w))) - screenCenter;
    px = dot(camera.getU(), screenPosition) + (imageWidth * 0.5);
    py = dot(-camera.getV(), screenPosition) + (imageHeight * 0.5);
    return result && ((px >= 0) && (px < imageWidth) && (py >= 0) && (py < imageHeight));
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
        // Outgoing vector from camera.
        Vector3 outgoing = path[1].point - path[0].point;
        const double dist2 = outgoing.magnitudeSquared();
        outgoing = outgoing * (1.0 / sqrt(dist2));
        const double c = dot(outgoing, camera.getW());
        const double ds2 = (camera.getDist(imageHeight) / c) * (camera.getDist(imageHeight) / c);
        W /= (c / ds2);
        color *= (W * std::abs(dot(outgoing, path[1].normal) / dist2));
      } else if (i == (path.length() - 1)) {
        bool isVertLight = dynamic_cast<DiffuseLight*>(&*(path[i].materialPtr));
        if (isVertLight) {
          // Incident direction to the last vertex (if it is on a light)
          const Vector3 incoming = (path[i - 1].point - path[i].point).normalized();
          const double L = path[i].materialPtr->brdf(incoming, path[i].normal, incoming);
          color *= path[i].materialPtr->getColor(path[i].interaction.uv, path[i].interaction.point) * L;
        } else {
          color *= 0.0;
        }
      } else {
        const Vector3 incoming = (path[i - 1].point - path[i].point).normalized();
        const Vector3 reflected = (path[i + 1].point - path[i].point).normalized();
        double BRDF = 0.0;
        if (path[i].materialPtr != nullptr) { BRDF = path[i].materialPtr->brdf(incoming, path[i].normal, reflected); }
        const UV uv = path[i].interaction.uv;
        const Point3 point = path[i].interaction.point;
        const Color materialColor = path[i].materialPtr->getColor(uv, point);
        const double geometryTermVal = geometryTerm(path[i], path[i + 1]);
        color *= materialColor * BRDF * geometryTermVal;
      }
      if (color.maxComponent() == 0.0) return color;
    }
    return color;
  }

  PathContribution combinePaths(const Path& eyePath, const Path& lightPath) const {
    PathContribution result(maxEvents);
    int maxPathLength = std::fmin(eyePath.length() + lightPath.length(), maxEvents);
    // maxEvents = the maximum number of vertices
    for (int pathLength = minPathLength; pathLength < maxPathLength; pathLength++) {
      for (int numEyeVertices = 1; numEyeVertices <= pathLength + 1; numEyeVertices++) {
        const int numLightVertices = (pathLength + 1) - numEyeVertices;

        if (numEyeVertices == 0) continue;  // no direct hit to the film (pinhole)
        if (numEyeVertices > eyePath.length()) continue;
        if (numLightVertices > lightPath.length()) continue;

        // extract subpaths
        Path eyeSubpath(eyePath.firstN(numEyeVertices));
        Path lightSubpath(lightPath.firstN(numLightVertices));

        // check the path visibility
        double px = -1.0, py = -1.0;
        if (!isConnectable(eyeSubpath, lightSubpath, px, py)) continue;

        // construct a full path
        Path sampledPath(pathLength);
        sampledPath.append(eyeSubpath);
        sampledPath.append(lightPath.reversed());

        // evaluate the path
        Color color = pathThroughput(sampledPath);
        double pathPDF = pathProbablityDensity(sampledPath, pathLength, numEyeVertices, numLightVertices);
        double weight = calculateMISWeight(sampledPath, numEyeVertices, numLightVertices, pathLength);
        if ((weight <= 0.0) || (pathPDF <= 0.0)) continue;

        auto scalar = (weight / pathPDF);
        if (std::isinf(scalar)) continue;
        color *= scalar;
        if (color.maxComponent() <= 0.0) continue;
        if (color.hasInfs()) continue;
        // if (color.hasNaNs()) continue;
        // if (color.hasInfs()) continue;

        // store the pixel contribution
        result.add(Contribution(color, px, py));

        // scalar contribution function
        result.scalarContrib = std::fmax(color.maxComponent(), result.scalarContrib);
      }
    }
    return result;
  }

  double calculateMISWeight(const Path& sampledPath, const int numEyeVertices, const int numLightVertices,
                            const int pathLength) const {
    const double sampledPathPDF = pathProbablityDensity(sampledPath, pathLength, numEyeVertices, numLightVertices);
    const double pathPDF = pathProbablityDensity(sampledPath, pathLength);
    if ((sampledPathPDF == 0.0) || (pathPDF == 0.0)) {
      return 0.0;
    } else {
      return std::fmax(std::fmin(sampledPathPDF / pathPDF, 1.0), 0.0);
    }
  }

  double lambertianPDF(const Vector3& wi, const Vector3& normal, const Vector3& wo) const {
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
          // Outgoing vector from camera.
          Vector3 outgoing = (sampledPath[1].point - sampledPath[0].point).normalized();
          double cosTheta = dot(outgoing, camera.getW());
          double distanceToScreen2 = camera.getDist(imageHeight) / cosTheta;
          distanceToScreen2 = distanceToScreen2 * distanceToScreen2;
          pdfValue /= (cosTheta / distanceToScreen2);

          pdfValue *= directionToArea(sampledPath[0], sampledPath[1]);
        } else {
          // PDF of sampling ith vertex
          Vector3 incoming = (sampledPath[i - 1].point - sampledPath[i].point).normalized();
          Vector3 reflected = (sampledPath[i + 1].point - sampledPath[i].point).normalized();

          if (sampledPath[i].materialPtr != nullptr) {
            pdfValue *= sampledPath[i].materialPtr->pdf(incoming, sampledPath[i].normal, reflected);
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
            // Outgoing vector from camera.
            Vector3 outgoing = (sampledPath[pathLength - 1].point - sampledPath[pathLength].point).normalized();
            pdfValue *= lambertianPDF(sampledPath[pathLength].normal, sampledPath[pathLength].normal, outgoing);
            pdfValue *= directionToArea(sampledPath[pathLength], sampledPath[pathLength - 1]);
          } else {
            // PDF of sampling (pathLength - i)th vertex
            Vector3 inc = (sampledPath[pathLength - (i - 1)].point - sampledPath[pathLength - i].point).normalized();
            Vector3 ref = (sampledPath[pathLength - (i + 1)].point - sampledPath[pathLength - i].point).normalized();

            if (sampledPath[pathLength - i].materialPtr != nullptr)
              pdfValue *= sampledPath[pathLength - i].materialPtr->pdf(inc, sampledPath[pathLength - i].normal, ref);

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