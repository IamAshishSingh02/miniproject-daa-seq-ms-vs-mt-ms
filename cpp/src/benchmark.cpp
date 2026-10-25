// Benchmark contract: JSON in, JSON out; timing excludes generation and
// formatting.
#include <bits/stdc++.h>
#include "mergesort.hpp"
using namespace std;
static size_t num(const string &s, const string &k, size_t d) {
  auto p = s.find("\"" + k + "\"");
  if (p == string::npos) return d;
  p = s.find(':', p);
  return stoull(s.substr(p + 1));
}
static string str(const string &s, const string &k, const string &d) {
  auto p = s.find("\"" + k + "\"");
  if (p == string::npos) return d;
  p = s.find(':', p);
  p = s.find('"', p) + 1;
  return s.substr(p, s.find('"', p) - p);
}
int main() {
  ostringstream q;
  q << cin.rdbuf();
  auto s = q.str();
  size_t n = num(s, "size", 10000), reps = max<size_t>(1, num(s, "repetitions", 3)), seed = num(s, "seed", 42), threads = num(s, "threads", 0);
  auto d = str(s, "distribution", "random");
  mt19937 g(seed);
  vector<int> base(n);
  if (d == "sorted")
    for (size_t i = 0; i < n; i++) base[i] = i;
  else if (d == "reverse")
    for (size_t i = 0; i < n; i++) base[i] = n - i;
  else if (d == "duplicates")
    for (auto &x : base) x = g() % 100;
  else
    for (auto &x : base) x = (int)g();
  vector<double> a, b;
  for (size_t r = 0; r < reps; r++) {
    auto x = base, y = base;
    auto t = chrono::steady_clock::now();
    mergesort::sequential(x);
    auto u = chrono::steady_clock::now();
    mergesort::parallel(y, threads);
    auto v = chrono::steady_clock::now();
    a.push_back(chrono::duration<double, milli>(u - t).count());
    b.push_back(chrono::duration<double, milli>(v - u).count());
    if (x != y) return 2;
  }
  auto avg = [](auto &x) {
    double z = 0;
    for (auto v : x) z += v;
    return z / x.size();
  };
  auto mn = [](auto &x) { return *min_element(x.begin(), x.end()); };
  auto mx = [](auto &x) { return *max_element(x.begin(), x.end()); };
  sort(a.begin(), a.end());
  sort(b.begin(), b.end());
  double sa = avg(a), pb = avg(b);
  cout << "{\"size\":" << n << ",\"seed\":" << seed << ",\"distribution\":\"" << d << "\",\"repetitions\":" << reps << ",\"sequential\":{"
       << "\"averageMs\":" << sa << ",\"medianMs\":" << a[a.size() / 2] << ",\"minMs\":" << mn(a) << ",\"maxMs\":" << mx(a) << "},\"parallel\":{"
       << "\"averageMs\":" << pb << ",\"medianMs\":" << b[b.size() / 2] << ",\"minMs\":" << mn(b) << ",\"maxMs\":" << mx(b)
       << "},\"correct\":true,\"speedup\":" << (pb ? sa / pb : 0) << ",\"improvementPercent\":" << (sa ? (sa - pb) / sa * 100 : 0) << "}\n";
}
