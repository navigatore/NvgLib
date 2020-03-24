#pragma once
#include <algorithm>
#include <chrono>
#include <random>

namespace nvg {
class Random {
 public:
  template <typename T>
  [[nodiscard]] std::vector<T> uniformVector(std::size_t n) {
    auto output = std::vector<T>(n, 0);
    auto maxElement = static_cast<T>(n - 1);
    std::uniform_int_distribution<T> dis(0, maxElement);
    std::generate(output.begin(), output.end(),
                  [this, &dis]() { return dis(gen); });
    return output;
  }

  template <typename T>
  [[nodiscard]] std::vector<T> permutationVector(std::size_t n) {
    auto v = std::vector<T>(n);
    std::generate(v.begin(), v.end(), []() {
      static auto value = T{};
      return value++;
    });
    std::shuffle(v.begin(), v.end(), gen);
    return v;
  }

 private:
  std::mt19937 gen{std::random_device{}()};
};

class Stopwatch {
 public:
  void start() noexcept {
    startTimePoint = std::chrono::high_resolution_clock::now();
  }
  void stop() noexcept {
    auto stopTimePoint = std::chrono::high_resolution_clock::now();
    duration += stopTimePoint - startTimePoint;
  }
  void reset() noexcept {
    duration = std::chrono::high_resolution_clock::duration::zero();
  }
  [[nodiscard]] auto getDurationInMs() const noexcept {
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
        .count();
  }

 private:
  std::chrono::high_resolution_clock::time_point startTimePoint{};
  std::chrono::high_resolution_clock::duration duration{};
};
}  // namespace nvg
