#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "GeometricalObject.hpp"
#include "Math.hpp"

class Sphere : public GeometricalObject {
 private:
  Point3F center;
  float radius;
  std::shared_ptr<Material> materialPtr;

 public:
  Sphere() {}
  Sphere(Point3F center, float radius, std::shared_ptr<Material> materialPtr) :
      center(center),
      radius(radius),
      materialPtr(materialPtr) {}

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    Vector3F oc = ray.origin - center;
    auto a = ray.direction.magnitudeSquared();
    auto halfB = dot(oc, ray.direction);
    auto c = oc.magnitudeSquared() - radius * radius;
    auto discriminant = halfB * halfB - a * c;

    auto root = std::sqrt(discriminant);
    auto t = (-halfB - root) / a;
    if (t < tMax && t > tMin) {
      interaction.t = t;
      interaction.point = ray.at(interaction.t);
      Vector3F outwardNormal = (interaction.point - center) / radius;
      interaction.setFaceNormal(ray, outwardNormal);
      interaction.materialPtr = materialPtr;
      interaction.uv = getUV(outwardNormal);
      return true;
    }
    t = (-halfB + root) / a;
    if (t < tMax && t > tMin) {
      interaction.t = t;
      interaction.point = ray.at(interaction.t);
      Vector3F outwardNormal = (interaction.point - center) / radius;
      interaction.setFaceNormal(ray, outwardNormal);
      interaction.uv = getUV(outwardNormal);
      interaction.materialPtr = materialPtr;
      return true;
    }
    return false;
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    // std::cout << "Bounding Box computed for:" << (this) << std::endl;
    outputBox = AABB(center - Vector3F(radius, radius, radius), center + Vector3F(radius, radius, radius));
    return true;
  }

  UV getUV(const Vector3F& outwardNormal) const {
    auto phi = std::atan2(outwardNormal.z, outwardNormal.x);
    auto theta = std::asin(outwardNormal.y);
    float u = 1 - (phi + Math::pi) / (2 * Math::pi);
    float v = (theta + Math::pi / 2) / Math::pi;
    return UV(u, v);
  }
};

#endif