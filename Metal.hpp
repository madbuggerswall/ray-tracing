#ifndef METAL_HPP
#define METAL_HPP

#include "Material.hpp"
#include "Ray.hpp"
#include "Utilities.hpp"
#include "Vector3.hpp"

class Metal : public Material {
 private:
  Color albedo;

 public:
  Metal(const Color& albedo) : albedo(albedo) {}
  virtual bool scatter(const Ray& incoming, const HitRecord& hitRecord,
                       Color& attenuation, Ray& scattered) const override {
    Vector3 incomingDirection = incoming.getDirection().normalized();
    Vector3 reflected = reflect(incomingDirection, hitRecord.normal);
    scattered = Ray(hitRecord.point, reflected);
    attenuation = albedo;
    return (dot(scattered.getDirection(), hitRecord.normal) > 0);
  }
};

#endif