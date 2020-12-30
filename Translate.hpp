#ifndef TRANSLATE_HPP
#define TRANSLATE_HPP

#include "GeometricalObject.hpp"

class Translate : public GeometricalObject {
 private:
  std::shared_ptr<GeometricalObject> object;
  Vector3F offset;

 public:
  Translate(std::shared_ptr<GeoObject> object, const Vector3F& offset) : object(object), offset(offset) {}
  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    const Ray movedRay(ray.origin - offset, ray.direction, ray.getTime());
    if (!object->intersect(movedRay, tMin, tMax, interaction)) return false;

    interaction.point += offset;
    interaction.setFaceNormal(movedRay, interaction.normal);
    return true;
  }
  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    if (!object->computeBoundingBox(t0, t1, outputBox)) return false;
    outputBox = AABB(outputBox.getMin() + offset, outputBox.getMax() + offset);
    return true;
  }
};

#endif