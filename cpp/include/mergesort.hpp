#pragma once
#include <bits/stdc++.h>
using namespace std;
namespace mergesort {
void sequential(vector<int> &a);
void parallel(vector<int> &a, unsigned max_threads = 0, size_t threshold = 2048);
}  // namespace mergesort
