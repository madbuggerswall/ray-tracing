#include <iostream>

#include "Camera.hpp"
#include "Configuration.hpp"
#include "Dielectric.hpp"
#include "Image.hpp"
#include "Integrator.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "MovingSphere.hpp"
#include "Scenes.hpp"
#include "Sphere.hpp"
#include "Stopwatch.hpp"

int main(int argc, char const* argv[]) {
  Stopwatch stopwatch;
  stopwatch.start();

  // Config
  CConfig config;
  Scene scene = Scenes::selectScene(8, config);
  Camera camera(config, 0.0, 1.0);
  Image image(config.imageHeight, config.imageWidth);

  // Rendering
  Sampler sampler(config);
  Integrator integrator(config);
  integrator.render(camera, scene, sampler, image);

  std::cout << std::endl << "Done." << std::endl;
  stopwatch.stop();
  stopwatch.printTime();

  std::cout << std::endl << "Writing image to file." << std::endl;
  stopwatch.start();
  std::string fileName;
  if (argc >= 2)
    fileName = argv[1];
  else
    fileName = "";
  image.writeToFile(fileName, config.samplesPerPixel);
  stopwatch.stop();
  stopwatch.printTime();
  return 0;
}
