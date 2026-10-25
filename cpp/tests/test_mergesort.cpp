#include <bits/stdc++.h>
#include "../include/mergesort.hpp"
using namespace std;
int main() {
  for (auto v : {vector<int>{}, vector<int>{1}, vector<int>{3, 1, 2}, vector<int>{1, 1, -2, 4, 0}, vector<int>{5, 4, 3, 2, 1}}) {
    auto e = v;
    mergesort::sequential(e);
    assert(is_sorted(e.begin(), e.end()));
    auto p = v;
    mergesort::parallel(p, 4);
    assert(e == p);
  }
  cout << "C++ merge sort tests passed\n";
}
