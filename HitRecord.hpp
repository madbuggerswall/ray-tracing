#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "Geometry/Point3.hpp"
#include "Geometry/Ray.hpp"
#include "Geometry/Vector3.hpp"

class Material;
struct HitRecord {
  Point3F point;
  Vector3F normal;
  UV uv;
  float t;
  bool frontFace;
  std::shared_ptr<Material> materialPtr;

  HitRecord() = default;

  // Copy constructor
  HitRecord(const HitRecord& other) :
      point(other.point),
      normal(other.normal),
      uv(other.uv),
      t(other.t),
      frontFace(other.frontFace),
      materialPtr(other.materialPtr) {}

  // Move constructor
  HitRecord(HitRecord&& other) :
      point(std::move(other.point)),
      normal(std::move(other.normal)),
      uv(std::move(other.uv)),
      t(std::exchange(other.t, 0)),
      frontFace(std::exchange(other.frontFace, 0)),
      materialPtr(std::move(other.materialPtr)) {}

  // Copy assignment
  HitRecord& operator=(const HitRecord& other) {
    point = other.point;
    normal = other.normal;
    uv = other.uv;
    t = other.t;
    frontFace = other.frontFace;
    materialPtr = other.materialPtr;
    return *this;
  }

  // Move assignment
  HitRecord& operator=(HitRecord&& other) {
    point = std::move(other.point);
    normal = std::move(other.normal);
    uv = other.uv;
    t = other.t;
    frontFace = other.frontFace;
    materialPtr = other.materialPtr;
    return *this;
  }

  inline void setFaceNormal(const Ray& ray, const Vector3F& outwardNormal) {
    frontFace = dot(ray.direction, outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

#endif