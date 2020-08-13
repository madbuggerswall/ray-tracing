#include <iostream>

#include "Camera.hpp"
#include "Color.hpp"
#include "Materials/Dielectric.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/Metal.hpp"
#include "MovingSphere.hpp"
#include "Scenes.hpp"
#include "Sphere.hpp"
#include "Utilities.hpp"

Color rayColor(const Ray& ray, const Color& background, const Scene& scene, int bounceLimit) {
  HitRecord record;
  if (bounceLimit <= 0) return Color(0, 0, 0);

  if (!scene.hit(ray, 0.001, Math::infinity, record)) return background;

  Ray scattered;
  Color attenuation;
  Color emission = record.materialPtr->emit(record.uv, record.point);

  double pdf;
  Color albedo;

  if (!record.materialPtr->scatter(ray, record, albedo, scattered, pdf)) return emission;

  Color result = emission + albedo * record.materialPtr->scatterPDF(ray, record, scattered);
  result *= rayColor(scattered, background, scene, bounceLimit - 1) / pdf;
  return result;
}

int main(int argc, char const* argv[]) {
  const auto aspectRatio = 1.0 / 1.0;
  const int imageWidth = 600;
  const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
  const int samplesPerPixel = 100;
  const int bounceLimit = 50;

  // World
  Color background(0, 0, 0);

  auto lights = std::make_shared<Scene>();
  RectangleXZ rectLight({213, 343, 227, 332}, 554, std::shared_ptr<Material>());
  Sphere sphereLight(Point3(190, 90, 190), 90, std::shared_ptr<Material>());
  lights->add(std::make_shared<RectangleXZ>(rectLight));
  lights->add(std::make_shared<Sphere>(sphereLight));

  auto scene = Scenes::cornellBox();

  // Camera

  Point3 lookFrom(278, 278, -800);
  Point3 lookAt(278, 278, 0);
  Vector3 viewUp(0, 1, 0);
  auto focusDist = 10.0;
  auto aperture = 0.0;
  auto verticalFOV = 40.0;
  auto t0 = 0.0;
  auto t1 = 1.0;

  Camera camera(lookFrom, lookAt, viewUp, verticalFOV, aspectRatio, aperture, focusDist, t0, t1);

  // Render
  std::cout << "P3" << std::endl;
  std::cout << imageWidth << "	" << imageHeight << std::endl;
  std::cout << "255" << std::endl;

  for (int j = imageHeight - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << "	" << std::flush;
    for (int i = 0; i < imageWidth; ++i) {
      Color pixelColor(0, 0, 0);
      for (int s = 0; s < samplesPerPixel; ++s) {
        auto u = double(i + Random::fraction()) / (imageWidth - 1);
        auto v = double(j + Random::fraction()) / (imageHeight - 1);
        Ray ray = camera.getRay(u, v);
        pixelColor += rayColor(ray, background, scene, bounceLimit);
      }
      writeColor(std::cout, pixelColor, samplesPerPixel);
    }
  }
  std::cerr << std::endl << "Done." << std::endl;

  return 0;
}
