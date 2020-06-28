#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "GeometricalObject.hpp"

class Sphere : GeometricalObject {
 private:
  Point3 center;
  double radius;

 public:
  Sphere() {}
  Sphere(Point3& center, double radius) : center(center), radius(radius) {}

  virtual bool hit(const Ray& ray,
                   double tMin,
                   double tMax,
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
      return true;
    }
    t = (-halfB + root) / a;
    if (t < tMax && t > tMin) {
      hitRecord.t = t;
      hitRecord.point = ray.at(hitRecord.t);
      Vector3 outwardNormal = (hitRecord.point - center) / radius;
      hitRecord.setFaceNormal(ray, outwardNormal);
      return true;
    }
    return false;
  }
};

#endif