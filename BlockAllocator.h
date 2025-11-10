#include <cstddef>

#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

class BlockAllocator {

public:
    BlockAllocator(size_t blockSize, size_t blockCount);

    ~BlockAllocator();

    void* allocate();

    void deallocate(void* p);

    size_t getBlockSize() const;

private:
    size_t m_blockSize;

    unsigned char* m_pool;

    void* m_freeList;

};

#endif //BLOCK_ALLOCATOR_H