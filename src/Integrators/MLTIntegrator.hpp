#ifndef MLT_INTEGRATOR_H
#define MLT_INTEGRATOR_H

#include "../PathUtils/MarkovChain.hpp"
#include "../PathUtils/PrimarySampleSpace.hpp"
#include "BDPIntegrator.hpp"

class MLTIntegrator : public BDPTIntegrator {
  mutable PrimarySampleSpace primarySampleSpace;
  const double largeStepProb = 0.3;
  const int numRNGsPerEvent = 2;
  const int numStatesSubpath = (maxEvents + 2) * numRNGsPerEvent;
  const int numStates = numStatesSubpath * 2;

 public:
  MLTIntegrator(const CConfig& config, const Scene& scene, const Camera& camera) :
      BDPTIntegrator(config, scene, camera) {}

  void tracePath(const Ray& ray, int bounceLimit, Path& path) const {
    if (bounceLimit <= 0) return;
    SurfaceInteraction interaction;

    // If a ray does not hit anything in the scene, return background color
    if (!scene.intersect(ray, 0.001, Math::infinity, interaction)) return;

    // set path data
    path.add(Vertex(interaction));

    const double random1 = primarySampleSpace[primarySampleSpace.offset];
    const double random2 = primarySampleSpace[primarySampleSpace.offset];
    Vector3 scatterDir = Random::cosineDirection(random1, random2);

    Ray scattered;
    Color attenuation;
    if (interaction.materialPtr->scatter(ray, interaction, attenuation, scattered, scatterDir)) {
      tracePath(scattered, bounceLimit - 1, path);
    }
  }

  Path generateCameraPath() const {
    Path path(maxEvents);
    primarySampleSpace.offset = 0;
    const double random1 = primarySampleSpace[primarySampleSpace.offset];
    const double random2 = primarySampleSpace[primarySampleSpace.offset];
    Ray ray = camera.getSample(imageHeight, imageWidth, random1, random2);
    path.add(Vertex(ray.origin, camera.getW()));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  Path generateCameraPath(ushort pixelX, ushort pixelY) const {
    Path path(maxEvents);
    primarySampleSpace.offset = 0;

    const double random1 = primarySampleSpace[primarySampleSpace.offset];
    const double random2 = primarySampleSpace[primarySampleSpace.offset];
    Ray ray = camera.getRay(sampler.getRandomSample(pixelX, pixelY, random1, random2));

    path.add(Vertex(ray.origin, camera.getW()));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  Path generateLightPath() const {
    Path path(maxEvents);
    auto randomLight = scene.getRandomLight();
    auto lightMaterial = scene.getRandomLight()->getMaterial();

    primarySampleSpace.offset = numStatesSubpath;

    auto origin = randomLight->samplePoint(primarySampleSpace[primarySampleSpace.offset],
                                           primarySampleSpace[primarySampleSpace.offset]);
    auto direction = Random::cosineDirection(primarySampleSpace[primarySampleSpace.offset],
                                             primarySampleSpace[primarySampleSpace.offset]);
    Ray ray(origin, direction);
    path.add(Vertex(ray.origin, ray.direction, lightMaterial));
    tracePath(ray, bounceLimit, path);
    return path;
  }

  void render(Image& image) const override {
    // initialize the Markov chain
    MarkovChain current(numStates, maxEvents);
    MarkovChain proposal(numStates, maxEvents);
    primarySampleSpace = PrimarySampleSpace(current);

    // Draw a candidate theta* from a proposal distribution q(theta*|theta_i-1)
    current.pathContribution = combinePaths(generateCameraPath(), generateLightPath());

    for (int j = imageHeight - 1; j >= 0; --j) {
      std::cout << "\rScanlines remaining: " << j << "  " << std::flush;
      for (int i = 0; i < imageWidth; ++i) {
        for (int s = 0; s < samplesPerPixel; ++s) {
          // sample the path
          double isLargeStepDone;
          if (Random::fraction() <= largeStepProb) {
            proposal = current.largeStep();
            isLargeStepDone = 1.0;
          } else {
            proposal = current.mutate(imageHeight, imageWidth);
            isLargeStepDone = 0.0;
          }
          primarySampleSpace = PrimarySampleSpace(proposal);
          proposal.pathContribution = combinePaths(generateCameraPath(), generateLightPath());

          double a = 1.0;
          if (current.pathContribution.scalarContrib > 0.0)
            a = std::fmax(
                std::fmin(1.0, proposal.pathContribution.scalarContrib / current.pathContribution.scalarContrib), 0.0);

          // accumulate samples
          if (proposal.pathContribution.scalarContrib > 0.0) {
            auto scale = (a + isLargeStepDone);
            scale /= (proposal.pathContribution.scalarContrib / normConstant + largeStepProb);
            proposal.pathContribution.accumulatePathContribution(scale, image);
          }

          //  It is worth using also the rejected samples since they also provide illumination information.
          if (current.pathContribution.scalarContrib > 0.0) {
            auto scale = (1.0 - a) / (current.pathContribution.scalarContrib / normConstant + largeStepProb);
            current.pathContribution.accumulatePathContribution(scale, image);
          }
          // update the chain
          if (Random::fraction() <= a) current = proposal;
        }
      }
    }
  }
};

#endif
