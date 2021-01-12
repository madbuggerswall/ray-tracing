#ifndef MARKOV_CHAIN_HPP
#define MARKOV_CHAIN_HPP

#include <vector>

#include "../Math/Random.hpp"
#include "PathContribution.hpp"

struct MarkovChain {
 private:
  int numStates;
  int maxEvents;
  std::vector<double> sampleSpace;

 public:
  PathContribution pathContribution;

  MarkovChain(int numStates, int maxEvents) : numStates(numStates), maxEvents(maxEvents), pathContribution(maxEvents) {
    sampleSpace.reserve(numStates);
    for (int i = 0; i < numStates; i++) sampleSpace.push_back(Random::fraction());
  }

  // Copy constructor
  MarkovChain(const MarkovChain& other) : sampleSpace(other.sampleSpace), pathContribution(other.pathContribution) {}
  // Move constructor
  MarkovChain(MarkovChain&& other) :
      sampleSpace(std::move(other.sampleSpace)),
      pathContribution(std::move(other.pathContribution)) {}
  // Copy assignment
  MarkovChain& operator=(const MarkovChain& other) {
    sampleSpace = other.sampleSpace;
    pathContribution = other.pathContribution;
    return *this;
  }
  // Move assignment
  MarkovChain& operator=(MarkovChain&& other) {
    sampleSpace = std::move(other.sampleSpace);
    pathContribution = std::move(other.pathContribution);
    return *this;
  }

  double& operator[](int index) { return sampleSpace[index]; }
  double operator[](int index) const { return sampleSpace[index]; }

  int length() const { return sampleSpace.size(); }

  // primary space Markov chain
  static inline double perturb(const double value, const double s1, const double s2) {
    double result;
    double randomValue = Random::fraction();
    if (randomValue < 0.5) {
      randomValue = randomValue * 2.0;
      result = value + s2 * std::exp(-std::log(s2 / s1) * randomValue);
      if (result > 1.0) result -= 1.0;
    } else {
      randomValue = (randomValue - 0.5) * 2.0;
      result = value - s2 * std::exp(-std::log(s2 / s1) * randomValue);
      if (result < 0.0) result += 1.0;
    }
    return result;
  }

  MarkovChain largeStep() const {
    MarkovChain result(numStates, maxEvents);
    result.pathContribution = pathContribution;
    return result;
  }

  MarkovChain mutate(int imageHeight, int imageWidth) const {
    MarkovChain result(numStates, maxEvents);
    result.pathContribution = pathContribution;

    // pixel location
    result.sampleSpace[0] = perturb(sampleSpace[0], 2.0 / double(imageHeight + imageWidth), 0.1);
    result.sampleSpace[1] = perturb(sampleSpace[1], 2.0 / double(imageHeight + imageWidth), 0.1);

    // the rest
    for (int i = 2; i < numStates; i++) result.sampleSpace[i] = perturb(sampleSpace[i], 1.0 / 1024.0, 1.0 / 64.0);
    return result;
  }
};

#endif