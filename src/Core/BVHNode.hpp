#ifndef BVH_NODE_HPP
#define BVH_NODE_HPP

#include <algorithm>

#include "../GeoObjects/GeometricalObject.hpp"
#include "../Math/Random.hpp"
#include "Scene.hpp"

inline bool boxCompare(const std::shared_ptr<GeoObject> a, const std::shared_ptr<GeoObject> b, int axis) {
  AABB boxA;
  AABB boxB;
  if (!a->computeBoundingBox(0, 0, boxA) || !b->computeBoundingBox(0, 0, boxB))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  return boxA.getMin()[axis] < boxB.getMin()[axis];
}
bool boxCompareX(const std::shared_ptr<GeoObject> a, const std::shared_ptr<GeoObject> b) { return boxCompare(a, b, 0); }
bool boxCompareY(const std::shared_ptr<GeoObject> a, const std::shared_ptr<GeoObject> b) { return boxCompare(a, b, 1); }
bool boxCompareZ(const std::shared_ptr<GeoObject> a, const std::shared_ptr<GeoObject> b) { return boxCompare(a, b, 2); }

// Bounding Volume Hierarch Node
class BVHNode : public GeometricalObject {
 private:
  std::shared_ptr<GeometricalObject> left;
  std::shared_ptr<GeometricalObject> right;
  AABB box;

 public:
  BVHNode() {}
  BVHNode(Scene& scene, double time0, double time1) :
      BVHNode(scene.getObjects(), 0, scene.getObjects().size(), time0, time1) {}
  BVHNode(std::vector<std::shared_ptr<GeoObject>>& objects, size_t start, size_t end, double time0, double time1) {
    int axis = Random::rangeInt(0, 2);
    auto comparator = (axis == 0) ? boxCompareX : (axis == 1) ? boxCompareY : boxCompareZ;
    const ushort objectSpan = end - start;

    if (objectSpan == 1) {
      left = right = objects[start];
    } else if (objectSpan == 2) {
      if (comparator(objects[start], objects[start + 1])) {
        left = objects[start];
        right = objects[start + 1];
      } else {
        left = objects[start + 1];
        right = objects[start];
      }
    } else {
      std::sort(objects.begin() + start, objects.begin() + end, comparator);
      auto mid = start + objectSpan / 2;
      left = std::make_shared<BVHNode>(objects, start, mid, time0, time1);
      right = std::make_shared<BVHNode>(objects, mid, end, time0, time1);
    }

    // Check whether there is a bounding box at all
    AABB boxLeft, boxRight;
    const bool isLeftBoxEmpty = !left->computeBoundingBox(time0, time1, boxLeft);
    const bool isRightBoxEmpty = !right->computeBoundingBox(time0, time1, boxRight);
    if (isLeftBoxEmpty || isRightBoxEmpty) { std::cerr << "No bounding box in BVHNode ctor." << std::endl; }
    box = AABB::surroundingBox(boxLeft, boxRight);
  }

  bool intersect(const Ray& ray, double tMin, double tMax, SInteraction& interaction) const override {
    if (!box.intersect(ray, tMin, tMax)) return false;
    const bool hitLeft = left->intersect(ray, tMin, tMax, interaction);
    const bool hitRight = right->intersect(ray, tMin, hitLeft ? interaction.t : tMax, interaction);
    return hitLeft || hitRight;
  }

  bool computeBoundingBox(double t0, double t1, AABB& outputBox) const override {
    outputBox = box;
    return true;
  }
  
	Point3 samplePoint() const override { return Point3(0, 0, 0); }
};

#endif