```markdown
# Custom Block Memory Allocator in C++

This project is a C++ implementation of a high-performance, fixed-size block (pool) memory allocator. It was developed to explore low-level memory management concepts, pointer manipulation, and efficient allocation strategies.

## 🎯 Purpose & Goals

The primary goal of this allocator is to provide a fast alternative to frequent calls to `new`/`delete` (or `malloc`/`free`). Instead of asking the OS for memory on every allocation, this allocator:

1. Acquires one large block of memory from the OS on initialization.
2. Manages that block by dividing it into a pool of equal-sized chunks.
3. Hands out these chunks to the user on request.
4. Takes chunks back when the user is done, returning them to a free list.

This model is useful for high-performance applications (for example, game engines or real-time systems) where the latency of standard memory allocation is a bottleneck.

## ✨ Core Features

- High performance: reduces allocation/deallocation overhead by avoiding system calls for each allocation.
- Low fragmentation: best suited for scenarios where objects of the same size are repeatedly allocated and freed.
- In-place free list: uses the memory inside freed blocks to store the free list pointers, so no extra memory is required for management.
- Robust & safe:
  - Returns `nullptr` on out-of-memory.
  - Enforces a minimum block size of at least `sizeof(void*)` to ensure safe free-list pointer storage.
- Clean C++ design: logic is encapsulated in a `BlockAllocator` class, with a header and implementation file.

## Files

- `BlockAllocator.h` — public interface.
- `BlockAllocator.cpp` — implementation (constructor, destructor, `allocate()`, `deallocate()`, `getBlockSize()`).
- `main.cpp` — test program and benchmark harness.

## 🛠️ Build and Run

The project targets C++11 and includes a small test suite and benchmark.

1. Compile

```bash
g++ main.cpp BlockAllocator.cpp -o my_allocator -std=c++11 -O2
```

For debugging or unoptimized behavior:

```bash
g++ main.cpp BlockAllocator.cpp -o my_allocator -std=c++11 -O0
```

2. Run

```bash
./my_allocator
```

## 🚀 Performance Benchmark

The `main.cpp` test suite runs two tests:

- Functional Test: Validates core allocator logic (allocation, deallocation, reuse, out-of-memory handling).
- Benchmark Test: Runs a high-stress steady-state churn benchmark to compare the custom allocator with the system allocator.

The steady-state churn test simulates a workload where a large number of objects are repeatedly replaced (allocated and freed), which is a worst-case for general-purpose allocators but a typical best-case for a pool allocator.

Example benchmark output (run on a 2024 Apple M3 MacBook Air in the original README):

```
========================================
  Custom Block Allocator Test Suite
========================================

--- Running Functional Test ---
... (functional test passes) ...
--- Functional Test Complete ---


--- Running Benchmark Test ---
Starting benchmark: 10000000 'replace' operations on a pool of 1000000 objects.

--- (Benchmark) 2.1: Standard 'new/delete' Churn ---
  Testing...

--- (Benchmark) 2.2: Custom BlockAllocator Churn ---
  (Using 1000000 blocks of size 8 bytes)
  Testing...

--- (Benchmark) 2.3: Final Results ---
    Standard 'new/delete' steady churn time: 756496 microseconds.
    Custom Allocator steady churn time:      111060 microseconds.

  In a STEADY STATE CHURN scenario, our allocator was 6.8116x faster.
--- Benchmark Test Complete ---

========================================
         All Tests Finished
========================================
```

Conclusion: In this steady-state churn scenario, the pool allocator shows a significant speedup versus repeated `new`/`delete`, demonstrating its usefulness for workloads with many short-lived, same-sized allocations.

## Notes & Next Steps

- This allocator is sized for fixed-size allocations. For variable sized allocations or more general-purpose needs, other allocators are more appropriate.
- If you want, I can:
  - open a PR with this README change,
  - add a short CONTRIBUTING or USAGE section,
  - or update the CI to build and run the test suite automatically.
```
