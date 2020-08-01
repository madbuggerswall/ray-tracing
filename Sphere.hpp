#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "GeometricalObject.hpp"

class Sphere : public GeometricalObject {
 private:
  Point3 center;
  double radius;
  std::shared_ptr<Material> materialPtr;

 public:
  Sphere() {}
  Sphere(Point3 center, double radius, std::shared_ptr<Material> materialPtr) :
      center(center),
      radius(radius),
      materialPtr(materialPtr) {}

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    Vector3 oc = ray.getOrigin() - center;
    auto a = ray.getDirection().magnitudeSquared();
    auto halfB = dot(oc, ray.getDirection());
    auto c = oc.magnitudeSquared() - radius * radius;
    auto discriminant = halfB * halfB - a * c;

    auto root = std::sqrt(discriminant);
    auto t = (-halfB - root) / a;
    if (t < tMax && t > tMin) {
      hitRecord.t = t;
      hitRecord.point = ray.at(hitRecord.t);
      Vector3 outwardNormal = (hitRecord.point - center) / radius;
      hitRecord.setFaceNormal(ray, outwardNormal);
      hitRecord.materialPtr = materialPtr;
      hitRecord.uv = getUV(outwardNormal);
      return true;
    }
    t = (-halfB + root) / a;
    if (t < tMax && t > tMin) {
      hitRecord.t = t;
      hitRecord.point = ray.at(hitRecord.t);
      Vector3 outwardNormal = (hitRecord.point - center) / radius;
      hitRecord.setFaceNormal(ray, outwardNormal);
      hitRecord.uv = getUV(outwardNormal);
      hitRecord.materialPtr = materialPtr;
      return true;
    }
    return false;
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    outputBox = AABB(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
    return true;
  }

  UV getUV(const Point3& point) const {
    auto phi = std::atan2(point.getZ(), point.getX());
    auto theta = std::asin(point.getY());
    double u = 1 - (phi + Math::pi) / (2 * Math::pi);
    double v = (theta + Math::pi / 2) / Math::pi;
    return UV(u, v);
  }
};

#endif