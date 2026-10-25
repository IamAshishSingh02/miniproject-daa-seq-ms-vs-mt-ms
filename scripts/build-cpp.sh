#!/usr/bin/env bash
set -euo pipefail
mkdir -p cpp/bin
g++ -std=c++17 -O2 -pthread cpp/src/mergesort.cpp cpp/src/benchmark.cpp -Icpp/include -o cpp/bin/mergesort-benchmark
