#ifndef ARGUMENT_PARSER_HPP
#define ARGUMENT_PARSER_HPP

#include <iostream>
#include <string>

#include "Configuration.hpp"

enum IntegratorType { Naive, Bidirectional, Metropolis };

// TODO: --help, --integrator
class ArgumentParser {
 private:
  const std::string argPrefix = "-";
  const std::string bounceSpec = "b";
  const std::string integratorSpec = "i";
  const std::string sampleSpec = "spp";
  const std::string sceneSpec = "s";
  const std::string fileNameSpec = "o";

  // Verbose arguments
  const std::string argPrefixVer = "--";
  const std::string bounceSpecVer = "bounce";
  const std::string integratorSpecVer = "integrator";
  const std::string sampleSpecVer = "sample";
  const std::string sceneSpecVer = "scene";
  const std::string fileNameSpecVer = "output";

  bool isNumerical(std::string string);
  void checkIntegratorArgument(std::string);

 public:
  ushort sceneSelection = 0;
  ushort samplesPerPixel = 0;
  ushort bounceLimit = 0;
  std::string fileName = "";
  IntegratorType integratorType = IntegratorType::Bidirectional;

  void parse(int argc, const char* argv[]);
  void setConfig(Config& config);
  void printInfo(const Config& config);
};

bool ArgumentParser::isNumerical(std::string string) {
  for (auto character : string) {
    if (!std::isdigit(character)) return false;
  }
  return true;
}

void ArgumentParser::checkIntegratorArgument(std::string token) {
  if (token.compare("naive") == 0)
    integratorType = IntegratorType::Naive;
  else if (token.compare("bdpt") == 0)
    integratorType = IntegratorType::Bidirectional;
  else if (token.compare("mlt") == 0)
    integratorType = IntegratorType::Metropolis;
}

void ArgumentParser::parse(int argc, const char* argv[]) {
  for (int i = 1; i < argc - 1; i++) {
    std::string token(argv[i]);
    std::string nextToken(argv[i + 1]);

    if (token.substr(0, 1).compare(argPrefix) == 0) {
      if (token.substr(1).compare(bounceSpec) == 0) {
        if (isNumerical(nextToken)) bounceLimit = std::stoi(nextToken);
      } else if (token.substr(1).compare(sampleSpec) == 0) {
        if (isNumerical(nextToken)) samplesPerPixel = std::stoi(nextToken);
      } else if (token.substr(1).compare(sceneSpec) == 0) {
        if (isNumerical(nextToken)) sceneSelection = std::stoi(nextToken);
      } else if (token.substr(1).compare(fileNameSpec) == 0) {
        fileName = nextToken;
      } else if (token.substr(1).compare(integratorSpec) == 0) {
        checkIntegratorArgument(nextToken);
      } else {
        std::cout << "No such parameter specifier. " << std::endl;
      }
    }

    // Verbose
    if (token.substr(0, 2).compare(argPrefixVer) == 0) {
      if (token.substr(2).compare(bounceSpecVer) == 0) {
        if (isNumerical(nextToken)) bounceLimit = std::stoi(nextToken);
      } else if (token.substr(2).compare(sampleSpecVer) == 0) {
        if (isNumerical(nextToken)) samplesPerPixel = std::stoi(nextToken);
      } else if (token.substr(2).compare(sceneSpecVer) == 0) {
        if (isNumerical(nextToken)) sceneSelection = std::stoi(nextToken);
      } else if (token.substr(2).compare(fileNameSpecVer) == 0) {
        fileName = nextToken;
      } else if (token.substr(1).compare(integratorSpecVer) == 0) {
        checkIntegratorArgument(nextToken);
      } else {
        std::cout << "No such parameter specifier. " << std::endl;
      }
    }
  }
}

void ArgumentParser::setConfig(Config& config) {
  if (bounceLimit != 0) config.bounceLimit = bounceLimit;
  if (samplesPerPixel != 0) config.samplesPerPixel = samplesPerPixel;
}

void ArgumentParser::printInfo(const Config& config) {
  auto integratorName = [this]() {
    if (integratorType == IntegratorType::Naive)
      return "Naive Path Tracer";
    else if (integratorType == IntegratorType::Bidirectional)
      return "Bidirectional Path Tracer";
    else if (integratorType == IntegratorType::Metropolis)
      return "Metropolis Light Transport";
    else
      return "Unknown";
  };

  std::cout << "\nINFO" << std::endl;
  std::cout << "Integrator:\t" << integratorName() << std::endl;
  std::cout << "Samples per pixel:\t" << config.samplesPerPixel << std::endl;
  std::cout << "Bounce limit:\t\t" << config.bounceLimit << std::endl;
  std::cout << "Dimension (h,w):\t" << config.imageHeight << "," << config.imageWidth << "\n\n";
}

#endif