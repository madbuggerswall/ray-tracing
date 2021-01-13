#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "../Math/Point3.hpp"
#include "../Math/Vector3.hpp"

struct CameraConfiguration {
  Point3 lookFrom;
  Point3 lookAt;
  Vector3 viewUp = Vector3(0, 1, 0);

  double verticalFOV = 40.0;
  double aspectRatio = 16.0 / 9.0;
  double aperture = 0.0;
  double focusDist = 10.0;

  size_t imageWidth = 400;
  size_t imageHeight = static_cast<size_t>(imageWidth / aspectRatio);
  Color background = Color(0, 0, 0);

  int samplesPerPixel = 1;
  int bounceLimit = 8;

  void printInfo() {
    std::cout << "\nINFO" << std::endl;
    std::cout << "Samples per pixel:\t" << samplesPerPixel << std::endl;
    std::cout << "Bounce limit:\t\t" << bounceLimit << std::endl;
    std::cout << "Dimension (h,w):\t" << imageHeight << "," << imageWidth << "\n\n";
  }
};

using CConfig = CameraConfiguration;
#endif