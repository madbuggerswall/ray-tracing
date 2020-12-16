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

  void setHitRecord(const Ray& ray, HitRecord& hitRecord, Vector3F outwardNormal, float x, float y, float t) const {
    hitRecord.uv = UV((x - corners[0]) / (corners[1] - corners[0]), (y - corners[2]) / (corners[3] - corners[2]));
    hitRecord.t = t;
    hitRecord.setFaceNormal(ray, outwardNormal);
    hitRecord.materialPtr = material;
    hitRecord.point = ray.at(t);
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    float padding = 0.0001;
    outputBox = AABB(Point3F(corners[0], corners[2], k - padding), Point3F(corners[1], corners[3], k + padding));
    return true;
  }
};

class RectangleXY : public Rectangle {
 public:
  RectangleXY() : Rectangle() {}
  RectangleXY(std::initializer_list<float> corners, float k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override {
    auto t = (k - ray.getOrigin().z) / ray.getDirection().z;
    if (t < tMin || t > tMax) return false;

    auto x = ray.getOrigin().x + t * ray.getDirection().x;
    auto y = ray.getOrigin().y + t * ray.getDirection().y;
    if (x < corners[0] || x > corners[1] || y < corners[2] || y > corners[3]) return false;

    setHitRecord(ray, hitRecord, Vector3F(0, 0, 1), x, y, t);
    return true;
  }
};

class RectangleXZ : public Rectangle {
 public:
  RectangleXZ() : Rectangle() {}
  RectangleXZ(std::initializer_list<float> corners, float k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override {
    auto t = (k - ray.getOrigin().y) / ray.getDirection().y;
    if (t < tMin || t > tMax) return false;

    auto x = ray.getOrigin().x + t * ray.getDirection().x;
    auto z = ray.getOrigin().z + t * ray.getDirection().z;
    if (x < corners[0] || x > corners[1] || z < corners[2] || z > corners[3]) return false;

    setHitRecord(ray, hitRecord, Vector3F(0, 1, 0), x, z, t);
    return true;
  }
};

class RectangleYZ : public Rectangle {
 public:
  RectangleYZ() : Rectangle() {}
  RectangleYZ(std::initializer_list<float> corners, float k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override {
    auto t = (k - ray.getOrigin().x) / ray.getDirection().x;
    if (t < tMin || t > tMax) return false;

    auto y = ray.getOrigin().y + t * ray.getDirection().y;
    auto z = ray.getOrigin().z + t * ray.getDirection().z;
    if (y < corners[0] || y > corners[1] || z < corners[2] || z > corners[3]) return false;

    setHitRecord(ray, hitRecord, Vector3F(1, 0, 0), y, z, t);
    return true;
  }
};

#endif