#include "BlockAllocator.h"
#include <cstddef>

BlockAllocator::BlockAllocator(size_t blockSize, size_t blockCount) {
    //Check if the user's blockSize is big enough to hold a pointer
    //If not, use sizeof(void*) as the minimum size.
    if (blockSize < sizeof(void*)) {
        m_blockSize = sizeof(void*);
    } else {
        m_blockSize = blockSize;
    }

    size_t totalSize = m_blockSize * blockCount;
    //We are now making an OS call for a block of memory of size totalSize
    m_pool = new unsigned char[totalSize]; 

    //To build the free list, we will implement a singly linked list where each node points to the next free block
    
    //Set the head of the list to the start of the pool
    m_freeList = m_pool;

    unsigned char* pCurrent = m_pool;
    //This loop makes every first 8 bytes of a free block point at the next free block
    for (size_t i = 0; i < blockCount - 1; ++i) {
        //Get the address of the next block
        unsigned char* pNext = pCurrent + m_blockSize;

        //pCurrent is a pointer to a byte, but we want it to point to an address, which needs 8 bytes
        //then casting (void**) makes the memory location a pointer to a void pointer
        //, which makes *(void**) a dereference to that pointer, meaning we are 'at' the block
        //We then store the next free address at those 8 bytes. 
        *(void**)pCurrent = pNext;

        pCurrent = pNext;
    }
    //The last block points to nothing
    *(void**)pCurrent = nullptr;
}

BlockAllocator::~BlockAllocator() {
    //The destructor only needs to free the single large pool of memory, 
    //regardless of the logic inside it
    delete[] m_pool;
}

void* BlockAllocator::allocate() {
    //Check if the structure has any blocks left
    if (m_freeList == nullptr) {
        return nullptr;
    }

    //Get the first free block, which will be handed to the user
    void* pBlock = m_freeList;
    
    //Update the free list to point at the next free block by treating pBlock as a pointer to a pointer, 
    //and then dereference it
    m_freeList = *(void**)pBlock;

    return pBlock;
}

void BlockAllocator::deallocate(void* p) {
    
    if (p == nullptr) {
        return;
    }

    //Turn the retrieved block to the head of the free list, 
    //Store a pointer to the old head of the list in it
    *(void**)p = m_freeList;
    m_freeList = p;
}

size_t BlockAllocator::getBlockSize() const {
    return m_blockSize;
}