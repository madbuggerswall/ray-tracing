#ifndef PRIMARY_SAMPLE_SPACE_HPP
#define PRIMARY_SAMPLE_SPACE_HPP

#include "MarkovChain.hpp"

class PrimarySampleSpace {
  std::vector<double> sampleSpace;

 public:
  mutable int offset;
  PrimarySampleSpace() = default;
  PrimarySampleSpace(int numStates) : offset(0) { sampleSpace.reserve(numStates); }
  PrimarySampleSpace(const MarkovChain& markovChain) : offset(0) {
    for (int i = 0; i < markovChain.length(); i++) sampleSpace.push_back(markovChain[i]);
  }

  // Copy constructor
  PrimarySampleSpace(const PrimarySampleSpace& other) : sampleSpace(other.sampleSpace), offset(other.offset) {}
  // Move constructor
  PrimarySampleSpace(PrimarySampleSpace&& other) :
      sampleSpace(std::move(other.sampleSpace)),
      offset(std::move(other.offset)) {}
  // Copy assignment
  PrimarySampleSpace& operator=(const PrimarySampleSpace& other) {
    sampleSpace = other.sampleSpace;
    offset = other.offset;
    return *this;
  }
  // Move assignment
  PrimarySampleSpace& operator=(PrimarySampleSpace&& other) {
    sampleSpace = std::move(other.sampleSpace);
    offset = std::move(other.offset);
    return *this;
  }

	// Every [] operation increments the offset for next sample
  double& operator[](int index) {
    offset++;
    return sampleSpace[index];
  }
  
	// Every [] operation increments the offset for next sample
	double operator[](int index) const {
    offset++;
    return sampleSpace[index];
  }
};
#endif