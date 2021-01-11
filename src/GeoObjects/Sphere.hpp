#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../Math/Math.hpp"
#include "GeometricalObject.hpp"

class Sphere : public GeometricalObject {
 private:
  Point3 center;
  float radius;
  std::shared_ptr<Material> material;

 public:
  Sphere() {}
  Sphere(Point3 center, float radius, std::shared_ptr<Material> material) :
      center(center),
      radius(radius),
      material(material) {}

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    Vector3 oc = ray.origin - center;
    const auto a = ray.direction.magnitudeSquared();
    const auto halfB = dot(oc, ray.direction);
    const auto c = oc.magnitudeSquared() - radius * radius;
    const auto discriminant = halfB * halfB - a * c;

    const auto root = std::sqrt(discriminant);
    auto t = (-halfB - root) / a;
    if (t < tMax && t > tMin) {
      interaction.t = t;
      interaction.point = ray.at(interaction.t);
      const Vector3 outwardNormal = (interaction.point - center) / radius;
      interaction.setFaceNormal(ray, outwardNormal);
      interaction.materialPtr = material;
      interaction.uv = getUV(outwardNormal);
      return true;
    }
    t = (-halfB + root) / a;
    if (t < tMax && t > tMin) {
      interaction.t = t;
      interaction.point = ray.at(interaction.t);
      const Vector3 outwardNormal = (interaction.point - center) / radius;
      interaction.setFaceNormal(ray, outwardNormal);
      interaction.uv = getUV(outwardNormal);
      interaction.materialPtr = material;
      return true;
    }
    return false;
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    // std::cout << "Bounding Box computed for:" << (this) << std::endl;
    outputBox = AABB(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
    return true;
  }

  std::shared_ptr<Material> getMaterial() const override { return material; }

  UV getUV(const Vector3& outwardNormal) const {
    const auto phi = std::atan2(outwardNormal.z, outwardNormal.x);
    const auto theta = std::asin(outwardNormal.y);
    const float u = 1 - (phi + Math::pi) / (2 * Math::pi);
    const float v = (theta + Math::pi / 2) / Math::pi;
    return UV(u, v);
  }
};

#endif