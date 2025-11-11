# Custom Block Memory Allocator in C++

This project is a C++ implementation of a high-performance, fixed-size block (or "pool") memory allocator. It was developed to master low-level memory management concepts, pointer manipulation, and C++ class design.

## 🎯 Purpose & Goals

The primary goal of this allocator is to provide a fast alternative to frequent calls to `new` and `delete` (or `malloc` and `free`). Instead of asking the operating system for memory for every new object, this allocator:

1.  **Acquires** one large block of memory from the OS on initialization.
2.  **Manages** this block by dividing it into a "pool" of equal-sized chunks.
3.  **Hands out** these chunks to the user upon request.
4.  **Takes back** the chunks when the user is done, adding them back to a free list.

This model is crucial for high-performance applications (like game engines or real-time systems) where the latency of standard memory allocation is a bottleneck.

## ✨ Core Features

  * **High Performance:** Drastically reduces allocation/deallocation overhead by eliminating system calls.
  * **Low Fragmentation:** Ideal for scenarios where objects of the same size are allocated and freed frequently.
  * **"In-Place" Free List:** Uses the storage of the free blocks themselves to build a singly-linked list, requiring no extra memory for management.
  * **Robust & Safe:**
      * Correctly handles "out-of-memory" by returning `nullptr`.
      * Silently enforces a minimum block size (`sizeof(void*)`) to prevent buffer overflows during free list management.
  * **Clean C++ Design:** Encapsulates all logic within a `BlockAllocator` class, separating the interface (`.h`) from the implementation (`.cpp`).

🛠️ How to Build and Run
The project is written in C++11 and includes a comprehensive test suite.

1. Compile
To get the most realistic benchmark, compile with optimizations turned on (-O2 is standard):

Bash

g++ main.cpp BlockAllocator.cpp -o my_allocator -std=c++11 -O2
If you want to debug or see un-optimized behavior, you can use -O0.

2. Run
Bash

./my_allocator
🚀 Performance Benchmark
The main.cpp file contains a test suite that runs two tests:

Functional Test: Validates the core logic (allocation, deallocation, re-use, and out-of-memory handling).

Benchmark Test: Proves the "optimized latency" claim by running a high-stress, realistic benchmark.

The "Steady State Churn" Test
This test simulates a real-world, high-performance application (like a game engine or server) that has a large number of active objects (POOL_SIZE) and is constantly allocating and deallocating them (NUM_OPERATIONS).

This "steady state churn" is a worst-case scenario for a general-purpose allocator (like new/delete) but the best-case scenario for a pool allocator.

Benchmark Results
Here is the output from running the benchmark on a 2024 Apple M3 MacBook Air, comparing our BlockAllocator against the system's highly-optimized new/delete:

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
Conclusion: In a realistic, high-stress scenario, this simple pool allocator is over 6.8 times faster than the standard system allocator, successfully demonstrating its value for high-performance code.llocator` class interface (API).
  * **`BlockAllocator.cpp`**: The implementation file. Contains the code for the constructor, destructor, `allocate()`, `deallocate()`, and `getBlockSize()`.
  * **`main.cpp`**: A simple test program that creates an allocator, runs a series of tests, and validates its behavior.
