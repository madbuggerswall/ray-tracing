#ifndef BOX_HPP
#define BOX_HPP


#include "../GeoObjects/AxisAlignedRectangle.hpp"
#include "../Core/Scene.hpp"

class Box : public GeometricalObject {
 private:
  Point3F min;
  Point3F max;
  Scene sides;

 public:
  Box() {}
  Box(const Point3F& min, const Point3F& max, std::shared_ptr<Material> material) : min(min), max(max) {
    const RectangleXY sideA({min.x, max.x, min.y, max.y}, max.z, material);
    const RectangleXY sideB({min.x, max.x, min.y, max.y}, min.z, material);
    const RectangleXZ sideC({min.x, max.x, min.z, max.z}, max.y, material);
    const RectangleXZ sideD({min.x, max.x, min.z, max.z}, min.y, material);
    const RectangleYZ sideE({min.y, max.y, min.z, max.z}, max.x, material);
    const RectangleYZ sideF({min.y, max.y, min.z, max.z}, min.x, material);

    sides.add(std::make_shared<RectangleXY>(sideA));
    sides.add(std::make_shared<RectangleXY>(sideB));
    sides.add(std::make_shared<RectangleXZ>(sideC));
    sides.add(std::make_shared<RectangleXZ>(sideD));
    sides.add(std::make_shared<RectangleYZ>(sideE));
    sides.add(std::make_shared<RectangleYZ>(sideF));
  }

  virtual bool intersect(const Ray& ray, float tMin, float tMax, SInteraction& interaction) const override {
    return sides.intersect(ray, tMin, tMax, interaction);
  }

  virtual bool computeBoundingBox(float t0, float t1, AABB& outputBox) const override {
    outputBox = AABB(min, max);
    return true;
  }
};

#endif