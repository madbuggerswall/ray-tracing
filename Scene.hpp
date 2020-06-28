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
  Scene(std::shared_ptr<GeoObject> object) {}

  void add(std::shared_ptr<GeoObject> object) {
    objects.push_back(object);
  }
  void clear() {
    objects.clear();
  }
  virtual bool hit(const Ray& ray, double tMin, double tMax,
                   HitRecord& hitRecord) const override {
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
};

#endif