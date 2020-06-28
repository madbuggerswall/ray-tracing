#include <iostream>

#include "Camera.hpp"
#include "Color.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Utilities.hpp"

Color rayColor(const Ray& ray, const Scene& scene) {
  HitRecord hitRecord;
  if (scene.hit(ray, 0, Util::infinity, hitRecord)) {
    return 0.5 * (hitRecord.normal + Color(1, 1, 1));
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

  std::cout << "P3" << std::endl;
  std::cout << imageWidth << "	" << imageHeight << std::endl;
  std::cout << "255" << std::endl;

  Scene scene;
  scene.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  scene.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera camera;

  for (int i = imageHeight - 1; i >= 0; --i) {
    std::cerr << "\rScanlines remaining: " << i << "	" << std::flush;
    for (int j = 0; j < imageWidth; ++j) {
      Color pixelColor(0, 0, 0);
      for (int s = 0; s < samplesPerPixel; ++s) {
        auto u = double(j + Util::randomDouble()) / (imageWidth - 1);
        auto v = double(i + Util::randomDouble()) / (imageHeight - 1);
        Ray ray = camera.getRay(u, v);
        pixelColor += rayColor(ray, scene);
      }
      writeColor(std::cout, pixelColor, samplesPerPixel);
    }
  }
  std::cerr << std::endl << "Done." << std::endl;

  return 0;
}
