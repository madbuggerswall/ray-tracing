#ifndef MOVING_SPHERE_HPP
#define MOVING_SPHERE_HPP

#include "../Core/AxisAlignedBoundingBox.hpp"
#include "GeometricalObject.hpp"

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
  MovingSphere(Point3 center0, Point3 center1, double time0, double time1, double radius,
               std::shared_ptr<Material> material) :
      center0(center0),
      center1(center1),
      time0(time0),
      time1(time1),
      radius(radius),
      material(material) {}

  virtual bool intersect(const Ray& ray, double tMin, double tMax, SInteraction& interaction) const override {
    const Vector3 oc = ray.origin - centerAt(ray.getTime());
    const auto a = ray.direction.magnitudeSquared();
    const auto halfB = dot(oc, ray.direction);
    const auto c = oc.magnitudeSquared() - radius * radius;

    const auto discriminant = halfB * halfB - a * c;

    if (discriminant > 0) {
      const auto root = std::sqrt(discriminant);
      auto t = (-halfB - root) / a;

      if (t < tMax && t > tMin) {
        interaction.t = t;
        interaction.point = ray.at(interaction.t);
        const auto outwardNormal = (interaction.point - centerAt(ray.getTime())) / radius;
        interaction.setFaceNormal(ray, outwardNormal);
        interaction.materialPtr = material;
        return true;
      }

      t = (-halfB + root) / a;
      if (t < tMax && t > tMin) {
        interaction.t = t;
        interaction.point = ray.at(interaction.t);
        const auto outwardNormal = (interaction.point - centerAt(ray.getTime())) / radius;
        interaction.setFaceNormal(ray, outwardNormal);
        interaction.materialPtr = material;
        return true;
      }
    }
    return false;
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    const Point3 minPoint = centerAt(t0) - Vector3(radius, radius, radius);
    const Point3 maxPoint = centerAt(t0) + Vector3(radius, radius, radius);
    const auto initBox = AABB(minPoint, maxPoint);
    const auto finBox = AABB(minPoint, maxPoint);
    outputBox = AABB::surroundingBox(initBox, finBox);
    return true;
  }
	
  std::shared_ptr<Material> getMaterial() const override { return material; }

  Point3 centerAt(double time) const { return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0); }
};

#endif