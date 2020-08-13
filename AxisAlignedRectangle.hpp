#ifndef AXIS_ALIGNED_RECTANGLE_HPP
#define AXIS_ALIGNED_RECTANGLE_HPP

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

  void setHitRecord(const Ray& ray, HitRecord& hitRecord, Vector3 outwardNormal, double x, double y, double t) const {
    hitRecord.uv = UV((x - corners[0]) / (corners[1] - corners[0]), (y - corners[2]) / (corners[3] - corners[2]));
    hitRecord.t = t;
    hitRecord.setFaceNormal(ray, outwardNormal);
    hitRecord.materialPtr = material;
    hitRecord.point = ray.at(t);
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    double padding = 0.0001;
    outputBox = AABB(Point3(corners[0], corners[2], k - padding), Point3(corners[1], corners[3], k + padding));
    return true;
  }
};

class RectangleXY : public Rectangle {
 public:
  RectangleXY() : Rectangle() {}
  RectangleXY(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    auto t = (k - ray.getOrigin().getZ()) / ray.getDirection().getZ();
    if (t < tMin || t > tMax) return false;

    auto x = ray.getOrigin().getX() + t * ray.getDirection().getX();
    auto y = ray.getOrigin().getY() + t * ray.getDirection().getY();
    if (x < corners[0] || x > corners[1] || y < corners[2] || y > corners[3]) return false;

    setHitRecord(ray, hitRecord, Vector3(0, 0, 1), x, y, t);
    return true;
  }
};

class RectangleXZ : public Rectangle {
 public:
  RectangleXZ() : Rectangle() {}
  RectangleXZ(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    auto t = (k - ray.getOrigin().getY()) / ray.getDirection().getY();
    if (t < tMin || t > tMax) return false;

    auto x = ray.getOrigin().getX() + t * ray.getDirection().getX();
    auto z = ray.getOrigin().getZ() + t * ray.getDirection().getZ();
    if (x < corners[0] || x > corners[1] || z < corners[2] || z > corners[3]) return false;

    setHitRecord(ray, hitRecord, Vector3(0, 1, 0), x, z, t);
    return true;
  }

  virtual double pdfValue(const Point3& origin, const Vector3& direction) const override {
    HitRecord hitRecord;
    if (!this->hit(Ray(origin, direction), 0.001, Math::infinity, hitRecord)) return 0;

    auto area = (corners[1] - corners[0]) * (corners[3] - corners[2]);
    auto distanceSquared = hitRecord.t * hitRecord.t * direction.magnitudeSquared();
    auto cosine = std::fabs(dot(direction, hitRecord.normal) / direction.magnitude());

    return distanceSquared / (cosine * area);
  }

  virtual Vector3 random(const Point3& origin) const override {
    auto randomPoint = Point3(Random::range(corners[0], corners[1]), k, Random::range(corners[2], corners[3]));
    return randomPoint - origin;
  }
};

class RectangleYZ : public Rectangle {
 public:
  RectangleYZ() : Rectangle() {}
  RectangleYZ(std::initializer_list<double> corners, double k, std::shared_ptr<Material> material) :
      Rectangle(corners, k, material) {}

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    auto t = (k - ray.getOrigin().getX()) / ray.getDirection().getX();
    if (t < tMin || t > tMax) return false;

    auto y = ray.getOrigin().getY() + t * ray.getDirection().getY();
    auto z = ray.getOrigin().getZ() + t * ray.getDirection().getZ();
    if (y < corners[0] || y > corners[1] || z < corners[2] || z > corners[3]) return false;

    setHitRecord(ray, hitRecord, Vector3(1, 0, 0), y, z, t);
    return true;
  }
};

#endif