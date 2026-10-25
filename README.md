<!-- Documentation includes local setup, API contract, methodology, complexity, and limitations. -->
# MergeSort Lab

An educational Next.js dashboard comparing real C++17 sequential and `std::thread` merge sort timings. It uses identical reproducible input data, warm-safe repeated runs, correctness checks, and a print-friendly report area.

## Run locally

Prerequisites: Node.js 20+, npm, and g++ 11+.

```bash
npm install
npm run build:cpp
npm run dev
```

Open http://localhost:3000. Build for production with `npm run build && npm start`; run C++ tests with `npm run test:cpp`.

The API is `POST /api/benchmark` with `{ "size": 10000, "distribution": "random", "seed": 42, "repetitions": 3, "threads": 0 }`. The Node route validates limits and invokes the fixed local executable without a shell. It is intended for a trusted local development environment; serverless platforms that cannot execute C++ are unsupported.

## Methodology and complexity

Generation and serialization are outside the timer. Every repetition sorts fresh copies and compares outputs. Both algorithms have O(n log n) total work and O(n) auxiliary array space; parallel execution additionally incurs thread-stack and synchronization overhead, so it may be slower for small inputs.

## Viva talking points

The parallel implementation recursively processes independent halves in a bounded thread budget, joins the worker before merging, and falls back to sequential sorting below a threshold. `join()` prevents merging partially sorted data; disjoint ranges avoid data races. Speedup is sequential time divided by parallel time.

## Limitations

Measurements vary by CPU load. Cancellation is timeout-based. The report uses browser print-to-PDF and only includes measurements actually collected in the current page.
