#include <algorithm>
#include <random>
namespace nvg {
class Random {
 public:
  template <typename T>
  std::vector<T> uniformVector(std::size_t n) {
    auto output = std::vector<T>(n, 0);
    auto maxElement = static_cast<T>(n - 1);
    std::uniform_int_distribution<T> dis(0, maxElement);
    std::generate(output.begin(), output.end(),
                  [this, &dis]() { return dis(gen); });
    return output;
  }

 private:
  std::mt19937 gen{std::random_device{}()};
};
}  // namespace nvg
