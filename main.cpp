#include <iostream>

#include "Camera.hpp"
#include "Color.hpp"
#include "Dielectric.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Utilities.hpp"

Scene randomScene() {
  using Random::fraction;
  Scene scene;

  auto groundMat = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  auto ground = std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMat);
  scene.add(ground);

  for (int i = -11; i < 11; ++i) {
    for (int j = -11; j < 11; ++j) {
      Point3 position(i + 0.9 * fraction(), 0.2, j + 0.9 * fraction());
      auto chooseMat = fraction();
      if ((position - Point3(4, 0.2, 0)).magnitude() > 0.9) {
        std::shared_ptr<Material> sphereMat;

        if (chooseMat < 0.8) {
          auto albedo = Random::vector3() * Random::vector3();
          sphereMat = std::make_shared<Lambertian>(albedo);
          auto sphere = std::make_shared<Sphere>(position, 0.2, sphereMat);
          scene.add(sphere);
        } else if (chooseMat < 0.95) {
          auto albedo = Random::vectorRange(0.5, 1.0);
          auto fuzz = Random::range(0.0, 0.5);
          sphereMat = std::make_shared<Metal>(albedo, fuzz);
          auto sphere = std::make_shared<Sphere>(position, 0.2, sphereMat);
          scene.add(sphere);
        } else {
          sphereMat = std::make_shared<Dielectric>(1.5);
          auto sphere = std::make_shared<Sphere>(position, 0.2, sphereMat);
          scene.add(sphere);
        }
      }
    }
  }

  auto mat1 = std::make_shared<Dielectric>(1.5);
  scene.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, mat1));

  auto mat2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  scene.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, mat2));

  auto mat3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  scene.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, mat3));

  return scene;
}

Color rayColor(const Ray& ray, const Scene& scene, int bounceLimit) {
  HitRecord record;
  if (bounceLimit <= 0) return Color(0, 0, 0);
  if (scene.hit(ray, 0.001, Math::infinity, record)) {
    Ray scattered;
    Color attenuation;
    if (record.materialPtr->scatter(ray, record, attenuation, scattered))
      return attenuation * rayColor(scattered, scene, bounceLimit - 1);
    return Color(0, 0, 0);
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

  Lambertian centerMat(Color(0.7, 0.3, 0.3));
  Sphere center(Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>(centerMat));
  scene.add(std::make_shared<Sphere>(center));

  Lambertian gMat(Color(0.8, 0.8, 0.0));
  Sphere ground(Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>(gMat));
  scene.add(std::make_shared<Sphere>(ground));

  Metal metalLeft(Color(0.8, 0.6, 0.2), 1.0);
  Sphere left(Point3(1, 0, -1), 0.5, std::make_shared<Metal>(metalLeft));
  scene.add(std::make_shared<Sphere>(left));

  Dielectric glass(1.7);
  Sphere right(Point3(-1, 0, -1), 0.5, std::make_shared<Dielectric>(glass));
  scene.add(std::make_shared<Sphere>(right));

  Dielectric glassI(1.7);
  Sphere rightI(Point3(-1, 0, -1), -0.3, std::make_shared<Dielectric>(glassI));
  scene.add(std::make_shared<Sphere>(rightI));

  // Metal metalRight(Color(0.8, 0.8, 0.8), 0.3);
  // Sphere right(Point3(-1, 0, -1), 0.5, std::make_shared<Metal>(metalRight));
  // scene.add(std::make_shared<Sphere>(right));

  auto lookFrom = Point3(3, 3, 2);
  auto lookAt = Point3(0, 0, -1);
  auto viewUp = Point3(0, 1, 0);
  auto focusDist = (lookFrom - lookAt).magnitude();
  auto aperture = 2.0;
  Camera camera(lookFrom, lookAt, viewUp, 20, aspectRatio, aperture, focusDist);

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
