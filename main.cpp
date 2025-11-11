// main.cpp
// Combines a functional test and a performance benchmark.

#include "BlockAllocator.h"
#include <iostream>
#include <vector>     // Needed for benchmark
#include <chrono>     // Needed for benchmark
#include <random>

// --- Function Declarations ---
void runFunctionalTest();
void runBenchmarkTest();

// --- Main Function ---
// This new main() will run both of our tests sequentially.
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Custom Block Allocator Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    
    std::cout << "\n--- Running Functional Test ---" << std::endl;
    runFunctionalTest();
    std::cout << "\n--- Functional Test Complete ---" << std::endl;
    
    std::cout << "\n\n--- Running Benchmark Test ---" << std::endl;
    runBenchmarkTest();
    std::cout << "--- Benchmark Test Complete ---" << std::endl;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "         All Tests Finished" << std::endl;
    std::cout << "========================================" << std::endl;
    return 0;
}


// --- Test 1: Functional Test ---
void runFunctionalTest() {
    //Blocks in this demo can hold exactly one integer.
    const size_t BLOCK_SIZE = sizeof(int);
    const size_t BLOCK_COUNT = 5;

    //Create an instance of the class
    BlockAllocator myAllocator(BLOCK_SIZE, BLOCK_COUNT);

    //The block size might have changed, 
    //this will happen on 64-bit machines where addresses are 8-bytes long.
    std::cout << "Creating allocator for " << BLOCK_COUNT 
    << " blocks of size " << myAllocator.getBlockSize() << " bytes." << std::endl;


    std::cout << "\n --- Test 1: Allocating all " 
                << BLOCK_COUNT << " blocks ---" << std::endl;
    
    //We are assigning an array of pointers to store the addresses of the free blocks
    void* pointers[BLOCK_COUNT];

    for (size_t i = 0; i < BLOCK_COUNT; ++i) {
        pointers[i] = myAllocator.allocate();
        if (pointers[i] != nullptr) {
            std::cout << "Allocated block " << i 
                        << " at address: " << pointers[i] << std::endl;
            
            //Key line: here, we dereference the address we just stored, and assign a new value to it
            //This proves the allocation to be successfull, since pointers was a void* array, 
            //so initially we coudn't write to it
            *(int*)pointers[i] = i * 10;
        }
    }
    
    std::cout << "\n--- Test 2: Allocating one more block (THIS SHOULD FAIL) ---" << std::endl;
    void* p_fail = myAllocator.allocate();
    if (p_fail == nullptr) {
        std::cout << "Correctly returned nullptr (out of assigned memory)." << std::endl;
    }

    std::cout << "\n--- Test 3: Deallocating block 2 ---" << std::endl;
    std::cout << "Returning address: " << pointers[2] << std::endl;
    myAllocator.deallocate(pointers[2]);

    std::cout << "Allocating again..." << std::endl;
    void* p_new = myAllocator.allocate();
    std::cout << "Got new address: " << p_new << std::endl;

    if (p_new == pointers[2]) {
        std::cout << "Success! The reallocated block is the same one." << std::endl;
    }
    
    // Destructor for myAllocator is called automatically here
}


// --- Test 2: Benchmark Test ---
// Final, un-optimizable "steady state" churn benchmark.

#include <vector>
#include <random> // For a real random index

void runBenchmarkTest() {
    // --- Configuration ---
    const size_t BLOCK_SIZE = sizeof(int);
    
    // The "pool" size. How many objects are alive at any given time.
    const size_t POOL_SIZE = 1000000;
    
    // The number of "replace" operations to time.
    const size_t NUM_OPERATIONS = 10000000;

    std::cout << "Starting benchmark: " << NUM_OPERATIONS 
              << " 'replace' operations on a pool of " << POOL_SIZE << " objects.\n" << std::endl;

    // A vector to hold our "live" objects
    std::vector<void*> pointer_pool(POOL_SIZE);

    // Setup a random number generator
    // This is the C++11 way to get high-quality random numbers
    std::mt19937 rng(12345); // Seeded for consistent results
    std::uniform_int_distribution<size_t> dist(0, POOL_SIZE - 1); // [0, POOL_SIZE-1]

    // =================================================================
    //  Test A: Standard 'new[]' / 'delete[]' Steady State Churn
    // =================================================================
    std::cout << "--- (Benchmark) 2.1: Standard 'new/delete' Churn ---" << std::endl;

    // 1. "Warm-up" / Fill the pool
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        pointer_pool[i] = new unsigned char[BLOCK_SIZE];
    }
    
    // 2. Timed Test: Replace random objects
    std::cout << "  Testing..." << std::endl;
    auto start_new_churn = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < NUM_OPERATIONS; ++i) {
        // Pick a random object to replace
        size_t index = dist(rng);
        
        // "delete" the old object
        delete[] static_cast<unsigned char*>(pointer_pool[index]);
        
        // "new" a replacement
        pointer_pool[index] = new unsigned char[BLOCK_SIZE];
    }

    auto end_new_churn = std::chrono::high_resolution_clock::now();
    auto duration_new_churn = std::chrono::duration_cast<std::chrono::microseconds>(end_new_churn - start_new_churn);

    // 3. Cleanup (not timed)
    for (void* p : pointer_pool) {
        delete[] static_cast<unsigned char*>(p);
    }
    pointer_pool.clear();
    pointer_pool.resize(POOL_SIZE); // Resize back for the next test

    // =================================================================
    //  Test B: Custom BlockAllocator Steady State Churn
    // =================================================================
    std::cout << "\n--- (Benchmark) 2.2: Custom BlockAllocator Churn ---" << std::endl;

    // 1. "Warm-up" / Fill the pool
    BlockAllocator myAllocator(BLOCK_SIZE, POOL_SIZE);
    std::cout << "  (Using " << POOL_SIZE << " blocks of size " 
              << myAllocator.getBlockSize() << " bytes)" << std::endl;
              
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        pointer_pool[i] = myAllocator.allocate();
    }
    
    // 2. Timed Test: Replace random objects
    std::cout << "  Testing..." << std::endl;
    // Reset the random generator for a fair test
    rng.seed(12345); 
    
    auto start_custom_churn = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < NUM_OPERATIONS; ++i) {
        // Pick a random object to replace
        size_t index = dist(rng);
        
        // "deallocate" the old object
        myAllocator.deallocate(pointer_pool[index]);
        
        // "allocate" a replacement
        pointer_pool[index] = myAllocator.allocate();
    }

    auto end_custom_churn = std::chrono::high_resolution_clock::now();
    auto duration_custom_churn = std::chrono::duration_cast<std::chrono::microseconds>(end_custom_churn - start_custom_churn);

    // 3. Cleanup (not timed) - Destructor handles it
    
    // =================================================================
    //  Test C: Final Results
    // =================================================================
    std::cout << "\n--- (Benchmark) 2.3: Final Results ---" << std::endl;
    std::cout << "    Standard 'new/delete' steady churn time: " << duration_new_churn.count() << " microseconds." << std::endl;
    std::cout << "    Custom Allocator steady churn time:      " << duration_custom_churn.count() << " microseconds." << std::endl;
    
    if (duration_custom_churn.count() > 0) {
        std::cout << "\n  In a STEADY STATE CHURN scenario, our allocator was " 
                  << (double)duration_new_churn.count() / duration_custom_churn.count() 
                  << "x faster." << std::endl;
    }
}
