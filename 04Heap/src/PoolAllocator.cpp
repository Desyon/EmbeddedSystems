#include "../include/PoolAllocator.h"

/**
 * Allocates the given amount of bytes if possible. If the needed amount is not available it returns null.
 * @param sizeInBytes amount of bytes memory needs to be allocated for.
 * @return pointer to the begin of the memory, if successful, else null.
 */
void *Heap::Allocate(size_t sizeInBytes) {
  // check if size for allocation is > 0 and if enough memory is available in the pool
  if ((static_cast<size_t>(0) == sizeInBytes) || (Available() < sizeInBytes)) {
    return nullptr;
  } else {
    for (size_t counter = 0; counter < blockCount; counter++) {
      if (!slice[counter].used) {
        slice[counter].used = true;
        return memory + counter * blockSize;
      }
    }
    return nullptr;
  }
}

/**
 * Deallocates the memory block at the given pointer. If the pointer is null or out of range it does nothing.
 * @param deleted address of the memory block to deallocate
 */
void Heap::Deallocate(void *deleted) {
  // nullcheck
  if (!deleted) return;
  // check if address is in memory pool
  if (deleted < memory || deleted >= &slice[blockCount]) return;

  uint8_t *casted = static_cast<uint8_t *>(deleted);

  unsigned long temp = casted - memory;

  if (!*casted % blockSize) {
    slice[temp/blockSize].used = false;
  }
  return;
}

/**
 * Returns the amount of bytes that is left in allocation pool.
 * @return bytes left to allocate.
 */
size_t Heap::Available() const {
  size_t available = 0;
  for (size_t counter = 0; counter < blockCount; counter++) {
    if (!slice[counter].used) {
      available += blockSize;
    }
  }

  return available;
}




