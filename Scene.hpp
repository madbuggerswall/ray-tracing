#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "GeometricalObject.hpp"

class Scene : public GeometricalObject {
 private:
  std::vector<std::shared_ptr<GeoObject>> objects;

 public:
  Scene() {}
  Scene(std::shared_ptr<GeoObject> object) { add(object); }

  void add(std::shared_ptr<GeoObject> object) { objects.push_back(object); }
  void clear() { objects.clear(); }

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    HitRecord record;
    bool hitAnything = false;
    auto closestSoFar = tMax;
    for (const auto& object : objects) {
      if (object->hit(ray, tMin, closestSoFar, record)) {
        hitAnything = true;
        closestSoFar = record.t;
        hitRecord = record;
      }
    }
    return hitAnything;
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    if (objects.empty()) return false;

    bool firstBox = true;
    AABB tempBox;

    for (const auto& object : objects) {
      if (!object->computeBoundingBox(t0, t1, tempBox)) return false;
      outputBox = firstBox ? tempBox : AABB::surroundingBox(outputBox, tempBox);
      firstBox = false;
    }
    return true;
  }
};

#endif