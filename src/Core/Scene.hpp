#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "../GeoObjects/AxisAlignedRectangle.hpp"
#include "../GeoObjects/GeometricalObject.hpp"
#include "../GeoObjects/Sphere.hpp"
#include "../Materials/Material.hpp"

class Scene : public GeometricalObject {
 private:
  std::vector<std::shared_ptr<GeoObject>> objects;
  std::vector<std::shared_ptr<GeoObject>> lights;

 public:
  Scene() {}
  Scene(std::shared_ptr<GeoObject> object) { add(object); }

  std::vector<std::shared_ptr<GeoObject>>& getObjects() { return objects; }

  void add(std::shared_ptr<GeoObject> object) {
    objects.push_back(object);
    if (object->getMaterial() != nullptr && dynamic_cast<DiffuseLight*>(&*(object->getMaterial())))
      lights.push_back(object);
  }
  void clear() { objects.clear(); }

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    SInteraction record;
    bool hitAnything = false;
    auto closestSoFar = tMax;
    for (const auto& object : objects) {
      if (object->intersect(ray, tMin, closestSoFar, record)) {
        hitAnything = true;
        closestSoFar = record.t;
        interaction = record;
      }
    }
    return hitAnything;
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
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

  virtual Point3 samplePoint() const override { return Point3(0, 0, 0); }

  std::shared_ptr<GeoObject> getRandomLight() const {
    if (lights.size() == 0)
      return nullptr;
    else if (lights.size() == 1)
      return lights[0];
    else
      return lights[Random::rangeInt(0, lights.size())];
  }

  std::vector<std::shared_ptr<GeoObject>> getLights() const { return lights; }
};

#endif