#pragma once
#include <vector>
namespace mergesort {
void sequential(std::vector<int> &a);
void parallel(std::vector<int> &a, unsigned max_threads = 0, std::size_t threshold = 2048);
}  // namespace mergesort
