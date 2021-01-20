#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>
#include <iostream>

using TimePoint = std::chrono::steady_clock::time_point;
class Stopwatch {
 private:
  TimePoint startPoint;
  TimePoint stopPoint;

  uint samples;
  std::chrono::milliseconds totalTime;

 public:
  Stopwatch() : samples(0), totalTime(0) {}
  void start() { startPoint = std::chrono::high_resolution_clock::now(); }
  void stop() { stopPoint = std::chrono::high_resolution_clock::now(); }
  int printTime() const {
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(stopPoint - startPoint);
    std::cout << seconds.count() << "s" << std::endl;
		return seconds.count();
  }

  void collectSample() {
    stop();
    totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(stopPoint - startPoint);
    samples++;
  }

  std::chrono::milliseconds getAverageTime() { return totalTime / samples; }
};
#endif