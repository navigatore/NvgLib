#pragma once
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <type_traits>

namespace nvg {
class Random {
 public:
  template <typename T>
  [[nodiscard]] std::vector<T> uniformVector(std::size_t n, T minElement,
                                             T maxElement) {
    static_assert(std::is_integral_v<T>,
                  "The type of stored values must be integral");
    auto output = std::vector<T>(n);
    std::uniform_int_distribution<T> dis(minElement, maxElement);
    std::generate(output.begin(), output.end(),
                  [this, &dis]() { return dis(gen); });
    return output;
  }

  template <typename T>
  [[nodiscard]] std::vector<T> uniformVector(std::size_t n) {
    return uniformVector(n, 0, static_cast<T>(n - 1));
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

template <typename T>
void printElementsInRange(const T first, const T last,
                          const std::string& separator = " ",
                          const std::string& ending = "\n") {
  for (auto i = first; i != last; ++i) {
    std::cout << *i << separator;
  }
  std::cout << ending;
}

constexpr auto defaultRepeatsPerSize = std::size_t{20};

template <typename C, typename G>
[[nodiscard]] std::vector<std::pair<std::size_t, double>> runtimeRatios(
    const C& callable, const G& dataGenerator, std::size_t maxN,
    std::size_t repeatsPerSize = defaultRepeatsPerSize) {
  auto ratios = std::vector<std::pair<std::size_t, double>>{};
  auto lastRuntime = std::chrono::milliseconds::zero().count();

  for (auto n = std::size_t{1}; n < maxN; n *= 2) {
    auto stopwatch = nvg::Stopwatch();
    for (auto i = std::size_t{0}; i < repeatsPerSize; ++i) {
      auto data = dataGenerator(n);
      stopwatch.start();
      callable(data);
      stopwatch.stop();
    }
    auto currentRuntime = stopwatch.getDurationInMs() / repeatsPerSize;
    if (lastRuntime > 0) {
      auto ratio = static_cast<double>(currentRuntime) / lastRuntime;
      ratios.emplace_back(n, ratio);
    }
    lastRuntime = currentRuntime;
  }

  return ratios;
}
}  // namespace nvg
