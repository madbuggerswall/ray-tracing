#include <iostream>

#include "Camera.hpp"
#include "Color.hpp"
#include "Dielectric.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
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

  if (!record.materialPtr->scatter(ray, record, attenuation, scattered)) return emission;

  return emission + attenuation * rayColor(scattered, background, scene, bounceLimit - 1);
}

int main(int argc, char const* argv[]) {
  // World
  Scene scene;
  Point3 lookFrom;
  Point3 lookAt;
  auto verticalFOV = 40.0;
  auto aperture = 0.0;
  Color background(0, 0, 0);
  int samplesPerPixel = 100;

  // Camera Config.
  double aspectRatio = 16.0 / 9.0;
  int imageWidth = 400;
  int imageHeight = static_cast<int>(imageWidth / aspectRatio);
  int bounceLimit = 50;
  auto focusDist = 10.0;
  Vector3 viewUp(0, 1, 0);

  // Scene Selection
  switch (8) {
    case 1:
      scene = Scenes::randomScene();
      background = Color(0.70, 0.80, 1.00);
      lookFrom = Point3(13, 2, 3);
      lookAt = Point3(0, 0, 0);
      verticalFOV = 20.0;
      aperture = 0.1;
      break;

    case 2:
      scene = Scenes::twoSpheres();
      background = Color(0.70, 0.80, 1.00);
      lookFrom = Point3(13, 2, 3);
      lookAt = Point3(0, 0, 0);
      verticalFOV = 20.0;
      break;

    case 3:
      scene = Scenes::twoPerlinSpheres();
      background = Color(0.70, 0.80, 1.00);
      lookFrom = Point3(13, 2, 3);
      lookAt = Point3(0, 0, 0);
      verticalFOV = 20.0;
      break;

    case 4:
      scene = Scenes::earth();
      background = Color(0.70, 0.80, 1.00);
      lookFrom = Point3(13, 2, 3);
      lookAt = Point3(0, 0, 0);
      verticalFOV = 20.0;
      break;

    case 5:
      scene = Scenes::simpleLight();
      samplesPerPixel = 400;
      background = Color(0, 0, 0);
      lookFrom = Point3(26, 3, 6);
      lookAt = Point3(0, 2, 0);
      verticalFOV = 20.0;
      break;

    case 6:
      scene = Scenes::cornellBox();
      aspectRatio = 1.0;
      imageWidth = 600;
      imageHeight = static_cast<int>(imageWidth / aspectRatio);
      samplesPerPixel = 200;
      background = Color(0, 0, 0);
      lookFrom = Point3(278, 278, -800);
      lookAt = Point3(278, 278, 0);
      verticalFOV = 40.0;
      break;

    case 7:
      scene = Scenes::cornellSmoke();
      aspectRatio = 1.0;
      imageWidth = 600;
      imageHeight = static_cast<int>(imageWidth / aspectRatio);
      samplesPerPixel = 200;
      lookFrom = Point3(278, 278, -800);
      lookAt = Point3(278, 278, 0);
      verticalFOV = 40.0;

    case 8:
      scene = Scenes::finalScene();
      aspectRatio = 1.0;
      imageWidth = 800;
      imageHeight = static_cast<int>(imageWidth / aspectRatio);
      samplesPerPixel = 200;
      background = Color(0, 0, 0);
      lookFrom = Point3(478, 278, -600);
      lookAt = Point3(278, 278, 0);
      verticalFOV = 40.0;
      break;

    default:
      scene = Scenes::randomScene();
      lookFrom = Point3(13, 2, 3);
      lookAt = Point3(0, 0, 0);
      verticalFOV = 20.0;
      aperture = 0.1;
      break;
  }

  Camera camera(lookFrom, lookAt, viewUp, verticalFOV, aspectRatio, aperture, focusDist, 0.0, 1.0);

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
