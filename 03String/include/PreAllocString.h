#ifndef PREALLOCSTRING_H
#define PREALLOCSTRING_H

#include <cstddef>

#include "Printf.h"

#define CREATE(varName, size) PreAllocString<size> varName

template<size_t MAX_SIZE>
class PreAllocString {
 private:
  const size_t max_size = MAX_SIZE;
  size_t length;

  char content[MAX_SIZE];

 public:
  /**
   * Creates a new empty instance of a PreAllocString
   */
  PreAllocString() {
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
    return length;
  }

  /**
   * Deletes all content of the string that is saved and resets the length value
   */
  void Empty() {
    content[0] = '\0';
    length = 0;
  }

  /**
   * Inserts a formatted string with the help of the Printf implementation from task 02
   * @param format Format string according to the limits for task 02
   * @param ... values for formatted input according to Printf implementation
   */
  void AddFormat(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *newLast = Printf(&content[length], (&content[0] + max_size), format, args);

    va_end(args);

    length = newLast - &content[0];
  }

  /**
   * Inserts a space in the saved string
   */
  void AddWhitespace() {
    char *newLast = Printf(&content[length], (&content[0] + max_size), " ");
    length = newLast - &content[0];
  }

  /*
   * Operators
   *
   * '='  <char>    --> sets content of this instance to <char>
   * '='  <String>  --> sets content of this instance to <String>
   *
   * '+=' <char>    --> appends <char> to this instance
   * '+=' <String>  --> appends <String> to this instance
   *
   * '[]' <index>   --> returns the char on the position of index
   */

  /**
   * Appends the char rhs
   * @param rhs char to append
   * @return this instance of the class
   */
  PreAllocString &operator=(char rhs) {
    Empty();
    AddFormat("%c", rhs);
    return *this;
  }

  /**
   * Sets the content to the string rhs
   * @param rhs the string to append
   * @return this instance of the class
   */
  PreAllocString &operator=(const char *rhs) {
    Empty();
    AddFormat("%s", rhs);
    return *this;
  }

  /**
   * Sets the content to the constant string rhs
   * @param rhs the string to be appended
   * @return this instance of the class
   */
  PreAllocString &operator=(char *const rhs) {
    Empty();
    AddFormat("%s", rhs);
    return *this;
  }

  /**
   * Appends the char to the content
   * @param rhs char to append
   * @return this instance of the class
   */
  PreAllocString &operator+=(char rhs) {
    AddFormat("%c", rhs);
    return *this;
  }

  /**
   * Appends the contents of the string to the existing content
   * @param rhs string to append
   * @return this instance of the class
   */
  PreAllocString &operator+=(char const *rhs) {
    AddFormat("%s", rhs);
    return *this;
  }

  /**
   * Defines the [] operator for PreAllocString
   * @param idx index of the char to be returned
   * @return the char on the position of idx if it exists, end of string otherwise
   */
  const char &operator[](const int idx) {
    if (max_size < idx || length < idx || 0 < idx) return '\0';
    return content[idx];
  }

  /**
   * Defines the cast of PreALlocString to a char *
   * @return the pointer to the first element of the content
   */
  operator const char *() const {
    return &content[0];
  }

  //TODO:
};

#endif