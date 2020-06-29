#include <iostream>

#include "Camera.hpp"
#include "Color.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Utilities.hpp"

Color rayColor(const Ray& ray, const Scene& scene, int bounceLimit) {
  HitRecord record;
  if (bounceLimit <= 0) return Color(0, 0, 0);
  if (scene.hit(ray, 0.001, Math::infinity, record)) {
    Point3 target = record.point + record.normal + Random::vectorInUnitSphere();
    Ray bouncingRay(record.point, target - record.point);
    return 0.5 * rayColor(bouncingRay, scene, bounceLimit - 1);
  }

  Vector3 unitDirection = ray.getDirection().normalized();
  auto t = 0.5 * (unitDirection.getY() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main(int argc, char const* argv[]) {
  const double aspectRatio = 16.0 / 9.0;
  const int imageWidth = 400;
  const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
  const int samplesPerPixel = 100;
	const int bounceLimit = 50;

  std::cout << "P3" << std::endl;
  std::cout << imageWidth << "	" << imageHeight << std::endl;
  std::cout << "255" << std::endl;

  Scene scene;
  scene.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  scene.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera camera;

  for (int j = imageHeight - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << "	" << std::flush;
    for (int i = 0; i < imageWidth; ++i) {
      Color pixelColor(0, 0, 0);
      for (int s = 0; s < samplesPerPixel; ++s) {
        auto u = double(i + Random::fraction()) / (imageWidth - 1);
        auto v = double(j + Random::fraction()) / (imageHeight - 1);
        Ray ray = camera.getRay(u, v);
        pixelColor += rayColor(ray, scene, bounceLimit);
      }
      writeColor(std::cout, pixelColor, samplesPerPixel);
    }
  }
  std::cerr << std::endl << "Done." << std::endl;

  return 0;
}
