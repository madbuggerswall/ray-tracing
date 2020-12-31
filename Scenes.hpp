#ifndef SCENES_HPP
#define SCENES_HPP
#include "AxisAlignedRectangle.hpp"
#include "BVHNode.hpp"
#include "Box.hpp"
#include "Configuration.hpp"
#include "ConstantMedium.hpp"
#include "Dielectric.hpp"
#include "DiffuseLight.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "MovingSphere.hpp"
#include "Rotate.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"
#include "Translate.hpp"

namespace Scenes {
  Scene randomScene() {
    using Random::fraction;
    Scene scene;

    // auto groundMat = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    // auto ground = std::make_shared<Sphere>(Point3F(0, -1000, 0), 1000, groundMat);
    // scene.add(ground);

    auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    scene.add(std::make_shared<Sphere>(Point3F(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

    for (int i = -11; i < 11; ++i) {
      for (int j = -11; j < 11; ++j) {
        Point3F position(i + 0.9 * fraction(), 0.2, j + 0.9 * fraction());
        auto chooseMat = fraction();
        if ((position - Point3F(4, 0.2, 0)).magnitude() > 0.9) {
          std::shared_ptr<Material> sphereMat;

          if (chooseMat < 0.8) {
            auto albedo = Random::vector() * Random::vector();
            sphereMat = std::make_shared<Lambertian>(albedo);
            auto center2 = position + Vector3F(0, Random::range(0, .5), 0);
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
    scene.add(std::make_shared<Sphere>(Point3F(0, 1, 0), 1.0, mat1));

    auto mat2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene.add(std::make_shared<Sphere>(Point3F(-4, 1, 0), 1.0, mat2));

    auto mat3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.add(std::make_shared<Sphere>(Point3F(4, 1, 0), 1.0, mat3));

    return scene;
  }

  Scene twoSpheres() {
    Scene scene;

    auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    scene.add(std::make_shared<Sphere>(Point3F(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    scene.add(std::make_shared<Sphere>(Point3F(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    return scene;
  }

  Scene twoPerlinSpheres() {
    Scene scene;

    auto perlineTexture = std::make_shared<PerlinTexture>(4);
    scene.add(std::make_shared<Sphere>(Point3F(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlineTexture)));
    scene.add(std::make_shared<Sphere>(Point3F(0, 2, 0), 2, std::make_shared<Lambertian>(perlineTexture)));

    return scene;
  }

  Scene earth() {
    Scene scene;

    auto earthTexture = std::make_shared<ImageTexture>("../Textures/earthmap.jpg");
    auto earthMat = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<Sphere>(Point3F(0, 0, 0), 2, earthMat);

    return Scene(globe);
  }

  Scene simpleLight() {
    Scene scene;

    auto perlinTexture = std::make_shared<PerlinTexture>(4);
    scene.add(std::make_shared<Sphere>(Point3F(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlinTexture)));
    scene.add(std::make_shared<Sphere>(Point3F(0, 2, 0), 2, std::make_shared<Lambertian>(perlinTexture)));

    auto diffuseLight = std::make_shared<DiffuseLight>(Color(4, 4, 4));
    RectangleXY rectangleXY({3, 5, 1, 3}, -2, diffuseLight);
    scene.add(std::make_shared<RectangleXY>(rectangleXY));

    return scene;
  }

  Scene cornellBox() {
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

    // Walls
    scene.add(std::make_shared<RectangleYZ>(rectangleA));
    scene.add(std::make_shared<RectangleYZ>(rectangleB));
    scene.add(std::make_shared<RectangleXZ>(rectangleC));
    scene.add(std::make_shared<RectangleXZ>(rectangleD));
    scene.add(std::make_shared<RectangleXZ>(rectangleE));
    scene.add(std::make_shared<RectangleXY>(rectangleF));

    std::shared_ptr<GeoObject> boxA = std::make_shared<Box>(Point3F(0, 0, 0), Point3F(165, 330, 165), white);
    boxA = std::make_shared<RotateY>(boxA, 15);
    boxA = std::make_shared<Translate>(boxA, Vector3F(265, 0, 295));
    scene.add(boxA);

    std::shared_ptr<GeoObject> boxB = std::make_shared<Box>(Point3F(0, 0, 0), Point3F(165, 165, 165), white);
    boxB = std::make_shared<RotateY>(boxB, -18);
    boxB = std::make_shared<Translate>(boxB, Vector3F(130, 0, 65));
    scene.add(boxB);
    return scene;
  }

  Scene cornellSmoke() {
    Scene scene;

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

    RectangleYZ sideA({0, 555, 0, 555}, 555, green);
    RectangleYZ sideB({0, 555, 0, 555}, 0, red);
    RectangleXZ sideC({113, 443, 127, 432}, 554, light);
    RectangleXZ sideD({0, 555, 0, 555}, 555, white);
    RectangleXZ sideE({0, 555, 0, 555}, 0, white);
    RectangleXY sideF({0, 555, 0, 555}, 555, white);

    scene.add(std::make_shared<RectangleYZ>(sideA));
    scene.add(std::make_shared<RectangleYZ>(sideB));
    scene.add(std::make_shared<RectangleXZ>(sideC));
    scene.add(std::make_shared<RectangleXZ>(sideD));
    scene.add(std::make_shared<RectangleXZ>(sideE));
    scene.add(std::make_shared<RectangleXY>(sideF));

    std::shared_ptr<GeoObject> boxA = std::make_shared<Box>(Point3F(0, 0, 0), Point3F(165, 330, 165), white);
    boxA = std::make_shared<RotateY>(boxA, 15);
    boxA = std::make_shared<Translate>(boxA, Vector3F(265, 0, 295));

    std::shared_ptr<GeoObject> boxB = std::make_shared<Box>(Point3F(0, 0, 0), Point3F(165, 165, 165), white);
    boxB = std::make_shared<RotateY>(boxB, -18);
    boxB = std::make_shared<Translate>(boxB, Vector3F(130, 0, 65));

    scene.add(std::make_shared<ConstantMedium>(boxA, 0.01, Color(0, 0, 0)));
    scene.add(std::make_shared<ConstantMedium>(boxB, 0.01, Color(1, 1, 1)));

    return scene;
  }

  Scene finalScene() {
    Scene scene;

    // Ground
    Scene ground;
    auto groundMat = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxesPerSide = 20;
    for (int i = 0; i < boxesPerSide; i++) {
      for (int j = 0; j < boxesPerSide; j++) {
        auto w = 100.0;
        auto x0 = -1000.0 + i * w;
        auto z0 = -1000.0 + j * w;
        auto y0 = 0.0;
        auto x1 = x0 + w;
        auto y1 = Random::range(1, 101);
        auto z1 = z0 + w;

        ground.add(std::make_shared<Box>(Point3F(x0, y0, z0), Point3F(x1, y1, z1), groundMat));
      }
    }

    scene.add(std::make_shared<BVHNode>(ground, 0, 1));

    // Light
    auto lightMat = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    RectangleXZ light({123, 423, 147, 412}, 554, lightMat);
    scene.add(std::make_shared<RectangleXZ>(light));

    // Moving sphere
    auto center1 = Point3F(400, 400, 200);
    auto center2 = center1 + Vector3F(30, 0, 0);
    auto moving_Sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    scene.add(std::make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_Sphere_material));

    scene.add(std::make_shared<Sphere>(Point3F(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    scene.add(std::make_shared<Sphere>(Point3F(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 10.0)));

    auto boundary = std::make_shared<Sphere>(Point3F(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
    scene.add(boundary);
    scene.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(Point3F(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    scene.add(std::make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    auto glassSphereA = std::make_shared<Sphere>(Point3F(120, 150, 145), 50, std::make_shared<Dielectric>(1.5));
    auto glassSphereB = std::make_shared<Sphere>(Point3F(120, 450, 145), 50, std::make_shared<Dielectric>(1.5));
    auto glassSphereC = std::make_shared<Sphere>(Point3F(120, 150, 0), 50, std::make_shared<Dielectric>(1.5));
    scene.add(glassSphereA);
    scene.add(glassSphereB);
    scene.add(glassSphereC);

    // Earth
    auto earthMat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("../Textures/earthmap.jpg"));
    scene.add(std::make_shared<Sphere>(Point3F(400, 200, 400), 100, earthMat));

    // Perlin Sphere
    auto perlinTexture = std::make_shared<PerlinTexture>(0.1);
    scene.add(std::make_shared<Sphere>(Point3F(220, 280, 300), 80, std::make_shared<Lambertian>(perlinTexture)));

    // Spheres
    Scene spheres;
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    int sphereCount = 160;
    for (int j = 0; j < sphereCount; j++) { spheres.add(std::make_shared<Sphere>(Random::pointRange(0, 165), 10, white)); }

    scene.add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(spheres, 0.0, 1.0), 15),
                                          Vector3F(-100, 270, 395)));

    return scene;
  }

  Scene selectScene(int index, CConfig& config) {
    Scene scene;
    switch (index) {
      case 1:
        scene = Scenes::randomScene();
        config.lookFrom = Point3F(13, 2, 3);
        config.lookAt = Point3F(0, 0, 0);
        config.verticalFOV = 20.0;
        config.aperture = 0.1;
        config.background = Color(0.70, 0.80, 1.00);
        break;

      case 2:
        scene = Scenes::twoSpheres();
        config.lookFrom = Point3F(13, 2, 3);
        config.lookAt = Point3F(0, 0, 0);
        config.verticalFOV = 20.0;
        config.background = Color(0.70, 0.80, 1.00);
        break;

      case 3:
        scene = Scenes::twoPerlinSpheres();
        config.lookFrom = Point3F(13, 2, 3);
        config.lookAt = Point3F(0, 0, 0);
        config.verticalFOV = 20.0;
        config.background = Color(0.70, 0.80, 1.00);
        break;

      case 4:
        scene = Scenes::earth();
        config.lookFrom = Point3F(13, 2, 3);
        config.lookAt = Point3F(0, 0, 0);
        config.verticalFOV = 20.0;
        config.background = Color(0.70, 0.80, 1.00);
        break;

      case 5:
        scene = Scenes::simpleLight();
        config.lookFrom = Point3F(26, 3, 6);
        config.lookAt = Point3F(0, 2, 0);
        config.verticalFOV = 20.0;
        config.aspectRatio = 1.0;
        config.imageWidth = 800;
        config.imageHeight = static_cast<int>(config.imageWidth / config.aspectRatio);
        config.background = Color(0, 0, 0);
        config.samplesPerPixel = 320;
        break;

      case 6:
        scene = Scenes::cornellBox();
        config.lookFrom = Point3F(278, 278, -800);
        config.lookAt = Point3F(278, 278, 0);
        config.verticalFOV = 40.0;
        config.aspectRatio = 1.0;
        config.imageWidth = 600;
        config.imageHeight = static_cast<int>(config.imageWidth / config.aspectRatio);
        config.background = Color(0, 0, 0);
        config.samplesPerPixel = 12;
        break;

      case 7:
        scene = Scenes::cornellSmoke();
        config.aspectRatio = 1.0;
        config.imageWidth = 600;
        config.imageHeight = static_cast<int>(config.imageWidth / config.aspectRatio);
        config.samplesPerPixel = 225;
        config.lookFrom = Point3F(278, 278, -800);
        config.lookAt = Point3F(278, 278, 0);
        config.verticalFOV = 40.0;

      case 8:
        scene = Scenes::finalScene();
        config.aspectRatio = 1.0;
        config.imageWidth = 800;
        config.imageHeight = static_cast<int>(config.imageWidth / config.aspectRatio);
        config.samplesPerPixel = 800;
        config.bounceLimit = 48;
        config.background = Color(0, 0, 0);
        config.lookFrom = Point3F(478, 278, -600);
        config.lookAt = Point3F(278, 278, 0);
        config.verticalFOV = 40.0;
        break;

      default:
        scene = Scenes::randomScene();
        config.lookFrom = Point3F(13, 2, 3);
        config.lookAt = Point3F(0, 0, 0);
        config.verticalFOV = 20.0;
        config.aperture = 0.1;
        break;
    }
    return scene;
  }
}

#endif