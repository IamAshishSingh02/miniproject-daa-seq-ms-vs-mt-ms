// Bounded parallel recursion with a sequential threshold keeps thread creation
// controlled.
#include "mergesort.hpp"
#include <algorithm>
#include <thread>
namespace mergesort {
static void merge(std::vector<int> &a, std::vector<int> &t, size_t l, size_t m, size_t r) {
  size_t i = l, j = m + 1, k = l;
  while (i <= m && j <= r) t[k++] = a[i] < a[j] ? a[i++] : a[j++];
  while (i <= m) t[k++] = a[i++];
  while (j <= r) t[k++] = a[j++];
  for (size_t x = l; x <= r; x++) a[x] = t[x];
}
static void seq(std::vector<int> &a, std::vector<int> &t, size_t l, size_t r) {
  if (l >= r) return;
  auto m = l + (r - l) / 2;
  seq(a, t, l, m);
  seq(a, t, m + 1, r);
  merge(a, t, l, m, r);
}
void sequential(std::vector<int> &a) {
  if (a.size() > 1) {
    std::vector<int> t(a.size());
    seq(a, t, 0, a.size() - 1);
  }
}
static void par(std::vector<int> &a, std::vector<int> &t, size_t l, size_t r, unsigned b, size_t th) {
  if (l >= r) return;
  if (r - l + 1 <= th || b < 2) {
    seq(a, t, l, r);
    return;
  }
  auto m = l + (r - l) / 2;
  std::thread x([&] { par(a, t, l, m, b / 2, th); });
  par(a, t, m + 1, r, b - b / 2, th);
  x.join();
  merge(a, t, l, m, r);
}
void parallel(std::vector<int> &a, unsigned b, size_t th) {
  if (a.size() < 2) return;
  if (!b) b = std::max(2u, std::thread::hardware_concurrency());
  std::vector<int> t(a.size());
  par(a, t, 0, a.size() - 1, b, th);
}
}  // namespace mergesort
