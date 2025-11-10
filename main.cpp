#include "BlockAllocator.h"
#include <iostream>

int main() {
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
    return 0;
}