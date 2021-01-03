#ifndef ARGUMENT_PARSER_HPP
#define ARGUMENT_PARSER_HPP

#include <iostream>
#include <string>

#include "Configuration.hpp"

// TODO: --help, --integrator
class ArgumentParser {
 private:
  const std::string argPrefix = "-";
  const std::string bounceSpec = "b";
  const std::string sampleSpec = "spp";
  const std::string sceneSpec = "s";
  const std::string fileNameSpec = "o";

  // Verbose arguments
  const std::string argPrefixVer = "--";
  const std::string bounceSpecVer = "bounce";
  const std::string sampleSpecVer = "sample";
  const std::string sceneSpecVer = "scene";
  const std::string fileNameSpecVer = "output";

  bool isNumerical(std::string string);

 public:
  ushort sceneSelection = 0;
  ushort samplesPerPixel = 0;
  ushort bounceLimit = 0;
  std::string fileName = "";

  void parse(int argc, const char* argv[]);
  void setConfig(CConfig& config);
};

bool ArgumentParser::isNumerical(std::string string) {
  for (auto character : string) {
    if (!std::isdigit(character)) return false;
  }
  return true;
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
      } else {
        std::cout << "No such parameter specifier. " << std::endl;
      }
    }
  }
}

void ArgumentParser::setConfig(CConfig& config) {
  if (bounceLimit != 0) config.bounceLimit = bounceLimit;
  if (samplesPerPixel != 0) config.samplesPerPixel = samplesPerPixel;
}
#endif