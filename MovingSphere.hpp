#ifndef MOVING_SPHERE_HPP
#define MOVING_SPHERE_HPP

#include "GeometricalObject.hpp"
#include "Utilities.hpp"

class MovingSphere : public GeometricalObject {
 private:
  Point3 center0;
  Point3 center1;
  double time0;
  double time1;
  double radius;
  std::shared_ptr<Material> material;

 public:
  MovingSphere() {}
  MovingSphere(
      Point3 center0,
      Point3 center1,
      double time0,
      double time1,
      double radius,
      std::shared_ptr<Material> material) :
      center0(center0),
      center1(center1),
      time0(time0),
      time1(time1),
      radius(radius),
      material(material) {}

  virtual bool hit(
      const Ray& ray,
      double tMin,
      double tMax,
      HitRecord& hitRecord) const override {
    Vector3 oc = ray.getOrigin() - center(ray.getTime());
    auto a = ray.getDirection().magnitudeSquared();
    auto halfB = dot(oc, ray.getDirection());
    auto c = oc.magnitudeSquared() - radius * radius;

    auto discriminant = halfB * halfB - a * c;

    if (discriminant > 0) {
      auto root = std::sqrt(discriminant);
      auto t = (-halfB - root) / a;

      if (t < tMax && t > tMin) {
        hitRecord.t = t;
        hitRecord.point = ray.at(hitRecord.t);
        auto outwardNormal = (hitRecord.point - center(ray.getTime())) / radius;
        hitRecord.setFaceNormal(ray, outwardNormal);
        hitRecord.materialPtr = material;
        return true;
      }

      t = (-halfB + root) / a;
      if (t < tMax && t > tMin) {
        hitRecord.t = t;
        hitRecord.point = ray.at(hitRecord.t);
        auto outwardNormal = (hitRecord.point - center(ray.getTime())) / radius;
        hitRecord.setFaceNormal(ray, outwardNormal);
        hitRecord.materialPtr = material;
        return true;
      }
    }
    return false;
  }

  Point3 center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
  }
};

#endif