#ifndef FLIP_FACE_HPP
#define FLIP_FACE_HPP

#include "GeometricalObject.hpp"

class FlipFace : public GeometricalObject {
 private:
  std::shared_ptr<GeometricalObject> object;

 public:
  FlipFace(std::shared_ptr<GeoObject> object) : object(object) {}

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    if (!object->hit(ray, tMin, tMax, hitRecord)) return false;

    hitRecord.frontFace = !hitRecord.frontFace;
    return true;
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    return object->computeBoundingBox(t0, t1, outputBox);
  }
};

#endif