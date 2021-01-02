#include <iostream>

#include "../GeoObjects/MovingSphere.hpp"
#include "../GeoObjects/Sphere.hpp"
#include "../Integrators/Integrator.hpp"
#include "../Materials/Dielectric.hpp"
#include "../Materials/Lambertian.hpp"
#include "../Materials/Metal.hpp"
#include "ArgumentParser.hpp"
#include "Camera.hpp"
#include "Configuration.hpp"
#include "Image.hpp"
#include "Scenes.hpp"
#include "Stopwatch.hpp"

int main(int argc, char const* argv[]) {
  Stopwatch stopwatch;
  stopwatch.start();

  ArgumentParser argParser;
  argParser.parse(argc, argv);

  // Config
  CConfig config;
  Scene scene = Scenes::selectScene(argParser.sceneSelection, config);

  argParser.setConfig(config);
  config.printInfo();

  Camera camera(config, 0.0, 1.0);
  Image image(config.imageHeight, config.imageWidth);

  // Rendering
  Sampler sampler(config);
  PathIntegrator integrator(config, scene, camera);
  integrator.render(image);

  std::cout << std::endl << "Done." << std::endl;
  stopwatch.stop();
  stopwatch.printTime();

  // Write image to a file
  std::cout << std::endl << "Writing image to file." << std::endl;
  stopwatch.start();

  image.writeToFile(argParser.fileName, config.samplesPerPixel);

  stopwatch.stop();
  stopwatch.printTime();
  return 0;
}
