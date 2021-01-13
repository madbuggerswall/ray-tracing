#ifndef AXIS_ALIGNED_RECTANGLE_HPP
#define AXIS_ALIGNED_RECTANGLE_HPP

#include <vector>

#include "../Materials/DiffuseLight.hpp"
#include "../Math/ONB.hpp"
#include "../Math/Random.hpp"
#include "GeometricalObject.hpp"

class Rectangle : public GeometricalObject {
 protected:
  std::shared_ptr<Material> material;
  std::vector<double> corners;
  double k;

  Rectangle() {}
  Rectangle(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material) :
      corners(corners),
      k(k),
      material(material) {}

  void setInteraction(const Ray& ray, SInteraction& interaction, const Vector3 outwardNormal, double x, double y,
                      double t) const {
    interaction.uv = UV((x - corners[0]) / (corners[1] - corners[0]), (y - corners[2]) / (corners[3] - corners[2]));
    interaction.t = t;
    interaction.setFaceNormal(ray, outwardNormal);
    interaction.materialPtr = material;
    interaction.point = ray.at(t);
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    double padding = 0.0001;
    outputBox = AABB(Point3(corners[0], corners[2], k - padding), Point3(corners[1], corners[3], k + padding));
    return true;
  }

  // Returns a pointer to the rectangle material
  std::shared_ptr<Material> getMaterial() const override { return material; }
};

class RectangleXY : public Rectangle {
  Vector3 normal = Vector3(0, 0, 1);

 public:
  RectangleXY() : Rectangle() {}
  RectangleXY(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}
  RectangleXY(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material, int side) :
      Rectangle(corners, k, material) {
    normal *= side;
  }

  virtual bool intersect(const Ray& ray, double tMin, double tMax, SInteraction& interaction) const override {
    const double t = (k - ray.origin.z) / ray.direction.z;
    if (!(t > tMin) || !(t < tMax)) return false;

    const double x = ray.origin.x + t * ray.direction.x;
    const double y = ray.origin.y + t * ray.direction.y;
    if (x < corners[0] || x > corners[1] || y < corners[2] || y > corners[3]) return false;

    setInteraction(ray, interaction, Vector3(0, 0, 1), x, y, t);
    return true;
  }

  // Returns a random point on this light rectangle.
  Point3 samplePoint() const override {
    return Point3(Random::range(corners[0], corners[1]), Random::range(corners[2], corners[3]), k);
  }

  Ray sampleDirection() const override {
    Point3 origin = samplePoint();
    ONB orthonormalBasis(normal);
    Vector3 direction = orthonormalBasis.local(Random::cosineDirection());
    return Ray(origin, direction);
  }

  double getArea() const override { return std::abs(corners[0] - corners[1]) * std::abs(corners[2] - corners[3]); }
};

class RectangleXZ : public Rectangle {
  Vector3 normal = Vector3(0, 1, 0);

 public:
  RectangleXZ() : Rectangle() {}
  RectangleXZ(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}
  RectangleXZ(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material, int side) :
      Rectangle(corners, k, material) {
    normal *= side;
  }

  virtual bool intersect(const Ray& ray, double tMin, double tMax, SInteraction& interaction) const override {
    const double t = (k - ray.origin.y) / ray.direction.y;
    if (!(t > tMin) || !(t < tMax)) return false;

    const double x = ray.origin.x + t * ray.direction.x;
    const double z = ray.origin.z + t * ray.direction.z;
    if (x < corners[0] || x > corners[1] || z < corners[2] || z > corners[3]) return false;

    setInteraction(ray, interaction, Vector3(0, 1, 0), x, z, t);
    return true;
  }

  // Returns a random point on this light rectangle.
  Point3 samplePoint() const override {
    return Point3(Random::range(corners[0], corners[1]), k, Random::range(corners[2], corners[3]));
  }

  Point3 samplePoint(double random1, double random2) const override {
    auto x = Random::mapInterval(random1, corners[0], corners[1]);
    auto z = Random::mapInterval(random2, corners[2], corners[3]);
    return Point3(x, k, z);
  }

  Ray sampleDirection() const override {
    Point3 origin = samplePoint();
    ONB orthonormalBasis(normal);
    Vector3 direction = orthonormalBasis.local(Random::cosineDirection());
    return Ray(origin, direction);
  }

  Ray sampleDirection(double random1, double random2, double random3, double random4) const override {
    Point3 origin = samplePoint(random1, random2);
    ONB orthonormalBasis(normal);
    Vector3 direction = orthonormalBasis.local(Random::cosineDirection(random3, random4));
    return Ray(origin, direction);
  }

  double getArea() const override { return std::abs(corners[0] - corners[1]) * std::abs(corners[2] - corners[3]); }
};

class RectangleYZ : public Rectangle {
  Vector3 normal = Vector3(1, 0, 0);

 public:
  RectangleYZ() : Rectangle() {}
  RectangleYZ(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}
  RectangleYZ(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material, int side) :
      Rectangle(corners, k, material) {
    normal *= side;
  }

  virtual bool intersect(const Ray& ray, double tMin, double tMax, SInteraction& interaction) const override {
    const double t = (k - ray.origin.x) / ray.direction.x;
    if (!(t > tMin) || !(t < tMax)) return false;

    const double y = ray.origin.y + t * ray.direction.y;
    const double z = ray.origin.z + t * ray.direction.z;
    if (y < corners[0] || y > corners[1] || z < corners[2] || z > corners[3]) return false;

    setInteraction(ray, interaction, Vector3(1, 0, 0), y, z, t);
    return true;
  }

  // Returns a random point on this light rectangle.
  Point3 samplePoint() const override {
    return Point3(k, Random::range(corners[0], corners[1]), Random::range(corners[2], corners[3]));
  }

  Point3 samplePoint(double random1, double random2) const override {
    auto y = Random::mapInterval(random1, corners[0], corners[1]);
    auto z = Random::mapInterval(random2, corners[2], corners[3]);
    return Point3(k, y, z);
  }

  Ray sampleDirection() const override {
    Point3 origin = samplePoint();
    ONB orthonormalBasis(normal);
    Vector3 direction = orthonormalBasis.local(Random::cosineDirection());
    return Ray(origin, direction);
  }

  Ray sampleDirection(double random1, double random2, double random3, double random4) const override {
    Point3 origin = samplePoint(random1, random2);
    ONB orthonormalBasis(normal);
    Vector3 direction = orthonormalBasis.local(Random::cosineDirection(random3, random4));
    return Ray(origin, direction);
  }

  double getArea() const override { return std::abs(corners[0] - corners[1]) * std::abs(corners[2] - corners[3]); }
};
#endif