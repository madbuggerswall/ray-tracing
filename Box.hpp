#ifndef BOX_HPP
#define BOX_HPP

#include "Utilities.hpp"
#include "AxisAlignedRectangle.hpp"
#include "Scene.hpp"

class Box : public GeometricalObject {
 private:
  Point3 min;
  Point3 max;
  Scene sides;

 public:
  Box() {}
  Box(const Point3& min, const Point3& max, std::shared_ptr<Material> material) : min(min), max(max) {
    RectangleXY sideA({min.getX(), max.getX(), min.getY(), max.getY()}, max.getZ(), material);
    RectangleXY sideB({min.getX(), max.getX(), min.getY(), max.getY()}, min.getZ(), material);
    RectangleXZ sideC({min.getX(), max.getX(), min.getZ(), max.getZ()}, max.getY(), material);
    RectangleXZ sideD({min.getX(), max.getX(), min.getZ(), max.getZ()}, min.getY(), material);
    RectangleYZ sideE({min.getY(), max.getY(), min.getZ(), max.getZ()}, max.getX(), material);
    RectangleYZ sideF({min.getY(), max.getY(), min.getZ(), max.getZ()}, min.getX(), material);

    sides.add(std::make_shared<RectangleXY>(sideA));
    sides.add(std::make_shared<RectangleXY>(sideB));
    sides.add(std::make_shared<RectangleXZ>(sideC));
    sides.add(std::make_shared<RectangleXZ>(sideD));
    sides.add(std::make_shared<RectangleYZ>(sideE));
    sides.add(std::make_shared<RectangleYZ>(sideF));
  }

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    return sides.hit(ray, tMin, tMax, hitRecord);
  }

  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    outputBox = AABB(min, max);
    return true;
  }
};

#endif