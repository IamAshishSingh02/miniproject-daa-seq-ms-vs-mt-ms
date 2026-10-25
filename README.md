# MergeX

### A focused lab for real sequential and parallel sorting benchmarks

MergeX is an educational performance lab comparing a sequential Merge Sort with a bounded multithreaded C++17 implementation. Benchmark numbers come from a real local C++ executable.

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C) ![Next.js](https://img.shields.io/badge/Next.js-App%20Router-000000)

## What it demonstrates

- Reproducible runs using a seed and identical input copies.
- Sequential and bounded `std::thread` Merge Sort.
- Median, average, minimum, maximum, speedup, and percentage improvement.
- Random, sorted, reverse-sorted, and duplicate-heavy distributions.
- Correctness verification before results reach the dashboard.
- A compact dark burgundy/plum interface with responsive layouts.
- An `i` help button explaining test repetitions for new users.
- A green engine-status indicator when the local C++ executable is available.

> **Important:** timings are measured by C++ using `std::chrono`; they vary with CPU load and system configuration.

## Technology

| Layer | Technology |
| --- | --- |
| Frontend | Next.js App Router, React, TypeScript, CSS |
| Backend | Next.js Node.js Route Handler |
| Algorithms | C++17, `std::thread`, `std::chrono` |
| Build | `g++`, Bash build script |

## Quick start

Prerequisites: Node.js 20+, npm, and g++ with C++17 and pthread support.

```bash
npm install
npm run build:cpp
npm run dev
```

Open [http://localhost:3000](http://localhost:3000). The dashboard is branded **MergeX** and uses a dark burgundy, plum, and charcoal visual theme. For production, run `npm run build && npm start`.

Useful commands:

```bash
npm run build:cpp   # Compile the optimized benchmark executable
npm run test:cpp    # Run C++ correctness tests
npm run dev         # Start the development server
npm run build       # Create an optimized frontend build
```

## Dashboard controls

- **Input length** selects 1,000, 10,000, 100,000, or 500,000 values.
- **Data pattern** selects random, already sorted, reverse sorted, or duplicate-heavy data.
- **Test runs** is the number of fresh copies sorted for each implementation. More runs make the median less sensitive to one noisy measurement; the dashboard explains this through the adjacent `i` button.
- **C++ benchmark online** confirms that the local C++17 executable is ready.

## How the benchmark works

1. The backend validates size, distribution, repetitions, seed, and thread budget.
2. C++ generates one deterministic base dataset.
3. Each repetition sorts fresh copies.
4. Only sorting is timed; generation, copying, and JSON formatting are excluded.
5. Outputs are compared for correctness before statistics are returned.

Speedup is `sequential time / multithreaded time`. A speedup below `1.0` is valid: thread overhead exceeded the parallel work saved for that run. Parallel sorting is not guaranteed to be faster, especially for smaller arrays.

## API contract

`POST /api/benchmark`

```json
{"size":10000,"distribution":"random","seed":42,"repetitions":3,"threads":0}
```

Supported distributions are `random`, `sorted`, `reverse`, and `duplicates`. The route limits array size to 500,000 values and repetitions to 10, then invokes only the repository-built executable through `child_process.spawn`.

## Complexity

Both versions divide the array, recursively sort each half, and merge the halves. The parallel version joins its worker before merging, and falls back to sequential recursion below a threshold.

| Case | Sequential Merge Sort | Multithreaded Merge Sort |
| --- | --- | --- |
| Best | O(n log n) | O(n log n) total work |
| Average | O(n log n) | O(n log n) total work |
| Worst | O(n log n) | O(n log n) total work |
| Auxiliary space | O(n) | O(n), plus thread-stack overhead |

Parallel elapsed time depends on processor count, workload size, scheduling, synchronization, and thread-management overhead. It is not guaranteed to be faster.

## Viva talking points

- `join()` ensures the worker has finished sorting before merging begins.
- Separate recursive ranges and joining before combination prevent data races.
- Small inputs often expose overhead more than parallel speedup.
- A reproducible seed makes experiments repeatable on the same environment.

## Project structure

```text
cpp/include/mergesort.hpp      Public declarations
cpp/src/mergesort.cpp          Algorithm implementations
cpp/src/benchmark.cpp          JSON benchmark executable
cpp/tests/test_mergesort.cpp   Correctness tests
scripts/build-cpp.sh           C++ build script
src/app/page.tsx               Dashboard UI
src/app/api/benchmark/         Node.js API route
```

## Limitations

This is intended for a trusted local development environment. Platforms that cannot execute native C++ processes cannot host the benchmark route unchanged.

## Git workflow

```bash
git status
git add .
git commit -m "describe your change"
git push origin main
```

Never commit `.env` files, credentials, `node_modules`, or generated binaries.
