#include <vector>
#include <random>
#include <algorithm>
#include <execution>

double sumKRandomElements(const std::vector<double>& a, size_t k) {
    if (a.empty())
      return 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, a.size() - 1);

    double sum = 0.0;

    // Generate a vector of K random indices
    std::vector<size_t> randomIndices(k);
    std::generate(std::execution::par, randomIndices.begin(), randomIndices.end(), [&]() {
        return dist(gen);
    });

    // Sum up the elements at the random indices in parallel
    sum = std::transform_reduce(std::execution::par, randomIndices.begin(), randomIndices.end(), 0.0,
                                 std::plus<>(), [&](size_t index) {
                                     return a[index];
                                 });

    return sum;
}
