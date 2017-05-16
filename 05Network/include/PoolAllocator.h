#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <cstddef>
#include <cstdint>

class IHeap {
 public:
  virtual void *Allocate(size_t) = 0;

  virtual void Deallocate(void *) = 0;

  virtual size_t Available() const = 0;
};

// block wrapper for giving out only blocks of size n + knowing allocation state
struct Block {
  bool used;
};

class Heap : public IHeap {
 public:
  void *Allocate(size_t);

  void Deallocate(void *);

  size_t Available() const;

 protected:
  Heap(uint8_t *memory, Block *slice, size_t blockCount, size_t blockSize)
      : memory(memory), slice(slice), blockCount(blockCount), blockSize(blockSize) {

    // set up all blocks
    for (size_t counter = 0; counter < blockCount; counter++) {
      slice[counter].used = false;
    }
  }

 private:
  uint8_t *const memory;
  Block *slice;

  const size_t blockCount;
  const size_t blockSize;
};

template<size_t BLOCKCOUNT, size_t BLOCKSIZE>
class PoolAllocator : public Heap {
 public:
  PoolAllocator() : Heap(memory, slice, BLOCKCOUNT, BLOCKSIZE) {}

 private:
  uint8_t memory[BLOCKCOUNT * BLOCKSIZE] = {0};

  Block slice[BLOCKCOUNT];
};

#endif /* POOLALLOCATOR_H */