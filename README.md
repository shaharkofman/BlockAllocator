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

## 🛠️ How to Build and Run

The project is written in C++11 (to use `nullptr`). It can be compiled with `g++` or `clang++`.

### 1\. Compile

Navigate to the project directory and run:

```bash
g++ main.cpp BlockAllocator.cpp -o my_allocator -std=c++11
```

  * `main.cpp`: The test program.
  * `BlockAllocator.cpp`: The allocator's implementation.
  * `-o my_allocator`: The name of the final executable.
  * `-std=c++11`: Required for modern C++ features.

### 2\. Run

```bash
./my_allocator
```

### 3\. Example Output

You will see the output from the test program (`main.cpp`), which validates all functionality:

```
Creating allocator for 5 blocks of size 8 bytes.

 --- Test 1: Allocating all 5 blocks ---
Allocated block 0 at address: 0x1054e59f0
Allocated block 1 at address: 0x1054e59f8
Allocated block 2 at address: 0x1054e5a00
Allocated block 3 at address: 0x1054e5a08
Allocated block 4 at address: 0x1054e5a10

--- Test 2: Allocating one more block (THIS SHOULD FAIL) ---
Correctly returned nullptr (out of assigned memory).

--- Test 3: Deallocating block 2 ---
Returning address: 0x1054e5a00
Allocating again...
Got new address: 0x1054e5a00
Success! The reallocated block is the same one.
```

## 📁 Code Structure

  * **`BlockAllocator.h`**: The header file. Defines the `BlockAllocator` class interface (API).
  * **`BlockAllocator.cpp`**: The implementation file. Contains the code for the constructor, destructor, `allocate()`, `deallocate()`, and `getBlockSize()`.
  * **`main.cpp`**: A simple test program that creates an allocator, runs a series of tests, and validates its behavior.