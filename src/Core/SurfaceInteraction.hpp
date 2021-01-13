#ifndef SURFACE_INTERACTION_HPP
#define SURFACE_INTERACTION_HPP

#include "../Math/Point3.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Vector3.hpp"

class Material;
struct SurfaceInteraction {
  Point3 point;
  Vector3 normal;
  UV uv;
  double t;
  bool frontFace;
  std::shared_ptr<Material> materialPtr;

  SurfaceInteraction() = default;

  // Copy constructor
  SurfaceInteraction(const SurfaceInteraction& other) :
      point(other.point),
      normal(other.normal),
      uv(other.uv),
      t(other.t),
      frontFace(other.frontFace),
      materialPtr(other.materialPtr) {}

  // Move constructor
  SurfaceInteraction(SurfaceInteraction&& other) noexcept :
      point(std::move(other.point)),
      normal(std::move(other.normal)),
      uv(std::move(other.uv)),
      t(std::exchange(other.t, 0)),
      frontFace(std::exchange(other.frontFace, 0)),
      materialPtr(std::move(other.materialPtr)) {}

  // Copy assignment
  SurfaceInteraction& operator=(const SurfaceInteraction& other) {
    point = other.point;
    normal = other.normal;
    uv = other.uv;
    t = other.t;
    frontFace = other.frontFace;
    materialPtr = other.materialPtr;
    return *this;
  }

  // Move assignment
  SurfaceInteraction& operator=(SurfaceInteraction&& other) noexcept {
    point = std::move(other.point);
    normal = std::move(other.normal);
    uv = other.uv;
    t = other.t;
    frontFace = other.frontFace;
    materialPtr = other.materialPtr;
    return *this;
  }

  inline void setFaceNormal(const Ray& ray, const Vector3& outwardNormal) {
    frontFace = dot(ray.direction, outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};
using SInteraction = SurfaceInteraction;
#endif