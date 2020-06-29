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

  virtual bool hit(const Ray& ray, double tMin, double tMax,
                   HitRecord& hitRecord) const override {
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
      return true;
    }
    t = (-halfB + root) / a;
    if (t < tMax && t > tMin) {
      hitRecord.t = t;
      hitRecord.point = ray.at(hitRecord.t);
      Vector3 outwardNormal = (hitRecord.point - center) / radius;
      hitRecord.setFaceNormal(ray, outwardNormal);
      hitRecord.materialPtr = materialPtr;
      return true;
    }
    return false;
  }
};

#endif