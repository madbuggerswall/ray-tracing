#ifndef ARGUMENT_PARSER_HPP
#define ARGUMENT_PARSER_HPP

#include <iostream>
#include <string>

#include "Configuration.hpp"

class ArgumentParser {
 private:
  // Verbose arguments
  std::string argPrefix = "--";
  std::string bounceSpec = "bounce";
  std::string sampleSpec = "sample";
  std::string sceneSpec = "scene";
  std::string fileNameSpec = "output";

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
    if (token.substr(0, 2).compare(argPrefix) == 0) {
      if (token.substr(2).compare(bounceSpec) == 0) {
        if (isNumerical(nextToken)) bounceLimit = std::stoi(nextToken);
      } else if (token.substr(2).compare(sampleSpec) == 0) {
        if (isNumerical(nextToken)) samplesPerPixel = std::stoi(nextToken);
      } else if (token.substr(2).compare(sceneSpec) == 0) {
        if (isNumerical(nextToken)) sceneSelection = std::stoi(nextToken);
      } else if (token.substr(2).compare(fileNameSpec) == 0) {
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