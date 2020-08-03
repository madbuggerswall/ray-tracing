#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "AxisAlignedBoundingBox.hpp"
#include "AxisAlignedRectangle.hpp"
#include "Dielectric.hpp"
#include "DiffuseLight.hpp"
#include "GeometricalObject.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "MovingSphere.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"

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

  static Scene randomScene() {
    using Random::fraction;
    Scene scene;

    // auto groundMat = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    // auto ground = std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMat);
    // scene.add(ground);

    auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    scene.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

    for (int i = -11; i < 11; ++i) {
      for (int j = -11; j < 11; ++j) {
        Point3 position(i + 0.9 * fraction(), 0.2, j + 0.9 * fraction());
        auto chooseMat = fraction();
        if ((position - Point3(4, 0.2, 0)).magnitude() > 0.9) {
          std::shared_ptr<Material> sphereMat;

          if (chooseMat < 0.8) {
            auto albedo = Random::vector() * Random::vector();
            sphereMat = std::make_shared<Lambertian>(albedo);
            auto center2 = position + Vector3(0, Random::range(0, .5), 0);
            auto sphere = std::make_shared<MovingSphere>(position, center2, 0.0, 1.0, 0.2, sphereMat);
            scene.add(sphere);
          } else if (chooseMat < 0.95) {
            auto albedo = Random::vectorRange(0.5, 1.0);
            auto fuzz = Random::range(0.0, 0.5);
            sphereMat = std::make_shared<Metal>(albedo, fuzz);
            auto sphere = std::make_shared<Sphere>(position, 0.2, sphereMat);
            scene.add(sphere);
          } else {
            sphereMat = std::make_shared<Dielectric>(1.5);
            auto sphere = std::make_shared<Sphere>(position, 0.2, sphereMat);
            scene.add(sphere);
          }
        }
      }
    }

    auto mat1 = std::make_shared<Dielectric>(1.5);
    scene.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, mat1));

    auto mat2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, mat2));

    auto mat3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, mat3));

    return scene;
  }

  static Scene twoSpheres() {
    Scene scene;

    auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    scene.add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    scene.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    return scene;
  }

  static Scene twoPerlinSpheres() {
    Scene scene;

    auto perlineTexture = std::make_shared<PerlinTexture>(4);
    scene.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlineTexture)));
    scene.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perlineTexture)));

    return scene;
  }

  static Scene earth() {
    Scene scene;

    auto earthTexture = std::make_shared<ImageTexture>("../Textures/earthmap.jpg");
    auto earthMat = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earthMat);

    return Scene(globe);
  }

  static Scene simpleLight() {
    Scene scene;

    auto perlinTexture = std::make_shared<PerlinTexture>(4);
    scene.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlinTexture)));
    scene.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perlinTexture)));

    auto diffuseLight = std::make_shared<DiffuseLight>(Color(4, 4, 4));
    RectangleXY rectangleXY({3, 5, 1, 3}, -2, diffuseLight);
    scene.add(std::make_shared<RectangleXY>(rectangleXY));

    return scene;
  }

  static Scene cornellBox() {
    Scene scene;

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    RectangleYZ rectangleA({0, 555, 0, 555}, 555, green);
    RectangleYZ rectangleB({0, 555, 0, 555}, 0, red);
    RectangleXZ rectangleC({213, 343, 227, 332}, 554, light);
    RectangleXZ rectangleD({0, 555, 0, 555}, 0, white);
    RectangleXZ rectangleE({0, 555, 0, 555}, 555, white);
    RectangleXY rectangleF({0, 555, 0, 555}, 555, white);

    scene.add(std::make_shared<RectangleYZ>(rectangleA));
    scene.add(std::make_shared<RectangleYZ>(rectangleB));
    scene.add(std::make_shared<RectangleXZ>(rectangleC));
    scene.add(std::make_shared<RectangleXZ>(rectangleD));
    scene.add(std::make_shared<RectangleXZ>(rectangleE));
    scene.add(std::make_shared<RectangleXY>(rectangleF));

    return scene;
  }
};

#endif