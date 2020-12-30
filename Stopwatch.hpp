#include <chrono>
#include <iostream>

using TimePoint = std::chrono::steady_clock::time_point;
class Stopwatch {
 private:
  TimePoint startPoint;
  TimePoint stopPoint;

 public:
  Stopwatch() {}
  void start() { startPoint = std::chrono::high_resolution_clock::now(); }
  void stop() { stopPoint = std::chrono::high_resolution_clock::now(); }
  void printTime() {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(stopPoint - startPoint);
    std::cout << seconds.count() << "s" << std::endl;
  }
};