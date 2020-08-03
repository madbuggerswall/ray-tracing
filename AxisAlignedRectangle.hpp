#ifndef AXIS_ALIGNED_RECTANGLE_HPP
#define AXIS_ALIGNED_RECTANGLE_HPP

#include "GeometricalObject.hpp"
class RectangleXY : public GeometricalObject {
 private:
  std::shared_ptr<Material> material;
  std::vector<double> corners;
  double k;

 public:
  RectangleXY() {}
  RectangleXY(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> material) :
      k(k),
      material(material) {
    corners = {x0, x1, y0, y1};
  }

  virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override {
    auto t = (k - ray.getOrigin().getZ()) / ray.getDirection().getZ();
    if (t < tMin || t > tMax) return false;
    auto x = ray.getOrigin().getX() + t * ray.getDirection().getX();
    auto y = ray.getOrigin().getY() + t * ray.getDirection().getY();

    if (x < corners[0] || x > corners[1] || y < corners[2] || y > corners[3]) return false;

    hitRecord.uv = UV((x - corners[0]) / (corners[1] - corners[0]), (y - corners[2]) / (corners[3] - corners[2]));
    hitRecord.t = t;
    auto outwardNormal = Vector3(0, 0, 1);
    hitRecord.setFaceNormal(ray, outwardNormal);
    hitRecord.materialPtr = material;
    hitRecord.point = ray.at(t);
    return true;
  }
  virtual bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    outputBox = AABB(Point3(corners[0], corners[2], k - 0.0001), Point3(corners[1], corners[3], k + 0.0001));
    return true;
  }
};

#endif