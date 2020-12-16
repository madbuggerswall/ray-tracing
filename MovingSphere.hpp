#ifndef MOVING_SPHERE_HPP
#define MOVING_SPHERE_HPP

#include "AxisAlignedBoundingBox.hpp"
#include "GeometricalObject.hpp"


class MovingSphere : public GeometricalObject {
 private:
  Point3F center0;
  Point3F center1;
  float time0;
  float time1;
  float radius;
  std::shared_ptr<Material> material;

 public:
  MovingSphere() {}
  MovingSphere(Point3F center0,
               Point3F center1,
               float time0,
               float time1,
               float radius,
               std::shared_ptr<Material> material) :
      center0(center0),
      center1(center1),
      time0(time0),
      time1(time1),
      radius(radius),
      material(material) {}

  virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const override {
    Vector3F oc = ray.getOrigin() - centerAt(ray.getTime());
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
        auto outwardNormal = (hitRecord.point - centerAt(ray.getTime())) / radius;
        hitRecord.setFaceNormal(ray, outwardNormal);
        hitRecord.materialPtr = material;
        return true;
      }

      t = (-halfB + root) / a;
      if (t < tMax && t > tMin) {
        hitRecord.t = t;
        hitRecord.point = ray.at(hitRecord.t);
        auto outwardNormal = (hitRecord.point - centerAt(ray.getTime())) / radius;
        hitRecord.setFaceNormal(ray, outwardNormal);
        hitRecord.materialPtr = material;
        return true;
      }
    }
    return false;
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    auto initBox = AABB(centerAt(t0) - Vector3F(radius, radius, radius), centerAt(t0) + Vector3F(radius, radius, radius));
    auto finBox = AABB(centerAt(t0) - Vector3F(radius, radius, radius), centerAt(t0) + Vector3F(radius, radius, radius));
    outputBox = AABB::surroundingBox(initBox, finBox);
    return true;
  }

  Point3F centerAt(float time) const { return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0); }
};

#endif