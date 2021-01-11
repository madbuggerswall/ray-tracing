#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "../Core/Camera.hpp"
#include "../Core/Configuration.hpp"
#include "../Core/Image.hpp"
#include "../Core/Random.hpp"
#include "../Core/Sampler.hpp"
#include "../Core/Scene.hpp"
#include "../Core/SurfaceInteraction.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Point3.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Vector3.hpp"
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
#endif