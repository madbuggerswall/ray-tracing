#include <iostream>

#include "Color.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"

double hitSphere(const Point3& center, double radius, const Ray& ray) {
  Vector3 oc = ray.getOrigin() - center;
  auto a = ray.getDirection().magnitudeSquared();
  auto halfB = dot(oc, ray.getDirection());
  auto c = oc.magnitudeSquared() - radius * radius;
  auto discriminant = halfB * halfB - a * c;
  if (discriminant < 0)
    return -1;
  else
    return (-halfB - sqrt(discriminant)) / a;
}

Color rayColor(const Ray& ray) {
  auto t = hitSphere(Point3(0, 0, -1), 0.5, ray);
  if (t > 0.0) {
    Vector3 normal = (ray.at(t) - Vector3(0, 0, -1)).normalized();
    return 0.5 * Color(normal.getX() + 1, normal.getY() + 1, normal.getZ() + 1);
  }
  Vector3 unitDirection = ray.getDirection().normalized();
  t = 0.5 * (unitDirection.getY() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main(int argc, char const* argv[]) {
  const float aspectRatio = 16.f / 9.f;
  const int imageWidth = 400;
  const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

  std::cout << "P3" << std::endl;
  std::cout << imageWidth << "	" << imageHeight << std::endl;
  std::cout << "255" << std::endl;

  auto viewportHeight = 2.0;
  auto viewportWidth = viewportHeight * aspectRatio;
  auto focalLength = 1.0;

  auto origin = Point3(0, 0, 0);
  auto horizontal = Vector3(viewportWidth, 0, 0);
  auto vertical = Vector3(0, viewportHeight, 0);
  auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2;
  lowerLeftCorner -= Vector3(0, 0, focalLength);

  for (int i = imageHeight - 1; i >= 0; --i) {
    std::cerr << "\rScanlines remaining: " << i << "	" << std::flush;
    for (int j = 0; j < imageWidth; ++j) {
      auto u = double(j) / (imageWidth - 1);
      auto v = double(i) / (imageHeight - 1);
      Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
      Color pixelColor = rayColor(ray);
      writeColor(std::cout, pixelColor);
    }
  }
  std::cerr << std::endl << "Done." << std::endl;

  return 0;
}
