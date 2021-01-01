#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "../Geometry/Point3.hpp"
#include "../Geometry/Vector3.hpp"

struct CameraConfiguration {
  Point3F lookFrom;
  Point3F lookAt;
  Vector3F viewUp = Vector3F(0, 1, 0);

  float verticalFOV = 40.0;
  float aspectRatio = 16.0 / 9.0;
  float aperture = 0.0;
  float focusDist = 10.0;
  
	size_t imageWidth = 400;
  size_t imageHeight = static_cast<size_t>(imageWidth / aspectRatio);
  Color background = Color(0, 0, 0);
	
  int samplesPerPixel = 24;
  int bounceLimit = 24;
};

using CConfig = CameraConfiguration;
#endif