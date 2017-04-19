#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H

#include <cstddef>

#include "Printf.h"

#define CREATE(varName, size) PreAllocString<size> varName

template<size_t MAX_SIZE>
class PreAllocString {
 private:
  const size_t max_size = MAX_SIZE;

  char *nextFree;
  char content[MAX_SIZE];

 public:
  /**
   * Creates a new empty instance of a PreAllocString
   */
  PreAllocString (){
    Empty();
  }

  /**
   * Returns the maximum allowed size for this instance of the PreAllocString
   * @return const size_t maximum size
   */
  constexpr size_t SizeOf() {
    return max_size;
  }

  /**
   * Returns the current length of the string that is saved
   * @return size_t current length
   */
  size_t GetLength() {
    return &content[max_size] - nextFree;
  }

  /**
   * Deletes all content of the string that is saved and resets the length value
   */
  void Empty() {
    for (size_t i = 0; i < max_size; i++) {
      content[i] = '\0';
    }
    nextFree = &content[0];
  }

  /**
   * Inserts a formatted string with the help of the Printf implementation from task 02
   * @param format Format string according to the limits for task 02
   * @param ... values for formatted input according to Printf implementation
   */
  void AddFormat(const char *format, ...) {
    va_list args;
    va_start(args, format);

    nextFree = Printf(nextFree, (&content[0] + max_size), format, args);

    va_end(args);
  }

  /**
   * Inserts a space in the saved string
   */
  void AddWhitespace() {
    nextFree = Printf(nextFree, (&content[0] + max_size), " ");
  }

  //TODO: operator stuff
};

#endif