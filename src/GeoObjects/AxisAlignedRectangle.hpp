#ifndef AXIS_ALIGNED_RECTANGLE_HPP
#define AXIS_ALIGNED_RECTANGLE_HPP

#include <vector>

#include "GeometricalObject.hpp"

class Rectangle : public GeometricalObject {
 protected:
  std::shared_ptr<Material> material;
  std::vector<float> corners;
  float k;

  Rectangle() {}
  Rectangle(std::initializer_list<float> corners, float k, std::shared_ptr<Material> material) :
      corners(corners),
      k(k),
      material(material) {}

  void setHitRecord(const Ray& ray, SInteraction& interaction, const Vector3 outwardNormal, float x, float y,
                    float t) const {
    interaction.uv = UV((x - corners[0]) / (corners[1] - corners[0]), (y - corners[2]) / (corners[3] - corners[2]));
    interaction.t = t;
    interaction.setFaceNormal(ray, outwardNormal);
    interaction.materialPtr = material;
    interaction.point = ray.at(t);
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    float padding = 0.0001;
    outputBox = AABB(Point3(corners[0], corners[2], k - padding), Point3(corners[1], corners[3], k + padding));
    return true;
  }
};

class RectangleXY : public Rectangle {
 public:
  RectangleXY() : Rectangle() {}
  RectangleXY(std::initializer_list<float> corners, float k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    const auto t = (k - ray.origin.z) / ray.direction.z;
    if (t < tMin || t > tMax) return false;

    const auto x = ray.origin.x + t * ray.direction.x;
    const auto y = ray.origin.y + t * ray.direction.y;
    if (x < corners[0] || x > corners[1] || y < corners[2] || y > corners[3]) return false;

    setHitRecord(ray, interaction, Vector3(0, 0, 1), x, y, t);
    return true;
  }
};

class RectangleXZ : public Rectangle {
 public:
  RectangleXZ() : Rectangle() {}
  RectangleXZ(std::initializer_list<float> corners, float k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    const auto t = (k - ray.origin.y) / ray.direction.y;
    if (t < tMin || t > tMax) return false;

    const auto x = ray.origin.x + t * ray.direction.x;
    const auto z = ray.origin.z + t * ray.direction.z;
    if (x < corners[0] || x > corners[1] || z < corners[2] || z > corners[3]) return false;

    setHitRecord(ray, interaction, Vector3(0, 1, 0), x, z, t);
    return true;
  }
};

class RectangleYZ : public Rectangle {
 public:
  RectangleYZ() : Rectangle() {}
  RectangleYZ(std::initializer_list<float> corners, float k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    const auto t = (k - ray.origin.x) / ray.direction.x;
    if (t < tMin || t > tMax) return false;

    const auto y = ray.origin.y + t * ray.direction.y;
    const auto z = ray.origin.z + t * ray.direction.z;
    if (y < corners[0] || y > corners[1] || z < corners[2] || z > corners[3]) return false;

    setHitRecord(ray, interaction, Vector3(1, 0, 0), y, z, t);
    return true;
  }
};

#endif