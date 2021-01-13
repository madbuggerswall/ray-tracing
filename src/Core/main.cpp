#include <iostream>

#include "../GeoObjects/MovingSphere.hpp"
#include "../GeoObjects/Sphere.hpp"
#include "../Integrators/BDPIntegrator.hpp"
#include "../Integrators/MLTIntegrator.hpp"
#include "../Integrators/PathIntegrator.hpp"
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

	// Parse CLI arguments
  ArgumentParser argParser;
  argParser.parse(argc, argv);

  // Config
  Config config;
  Scene scene = Scenes::selectScene(argParser.sceneSelection, config);

  argParser.setConfig(config);
  argParser.printInfo(config);

  // Configure camera and image
  Camera camera(config, 0.0, 1.0);
  Image image(config.imageHeight, config.imageWidth);

  // Select integrator
  auto selectIntegrator = [config, scene, camera](IntegratorType type) -> std::shared_ptr<Integrator> {
    if (type == IntegratorType::Naive)
      return std::make_shared<PathIntegrator>(config, scene, camera);
    else if (type == IntegratorType::Bidirectional)
      return std::make_shared<BDPTIntegrator>(config, scene, camera);
    else if (type == IntegratorType::Metropolis)
      return std::make_shared<MLTIntegrator>(config, scene, camera);
    else
      return std::make_shared<BDPTIntegrator>(config, scene, camera);
  };

  // Rendering
  std::shared_ptr<Integrator> integrator = selectIntegrator(argParser.integratorType);
  integrator->render(image);

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
