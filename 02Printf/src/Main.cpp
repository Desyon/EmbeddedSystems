#include "../include/Printf.h"
#include <iostream>

int main() {
  char buffer[1024];
  char tinyBuffer[10];

  unsigned int test = 42;

  int success = 0;

  success = Printf(buffer, buffer + 1023,
                   "Forty Two %d %u %c %s %x %b %%", -42, test, '*', "Forty Two", 42, 42) ? 1 : 0;
  if (success) {
    std::cout << "Test 1:\t" << buffer << "\n";
  } else {
    std::cout << "Test 1:\tfailed\n";
  }

  success = Printf(tinyBuffer, tinyBuffer + 9, "A String That is too long for the buffer") ? 0 : 1;
  if (success) {
    std::cout << "Test 2:\tsuccess\n";
  } else {
    std::cout << "Test 2:\tfailed\n";
  }

  success = Printf(buffer, buffer + 1023, "%w %%%") ? 1 : 0;
  if (success) {
    std::cout << "Test 3:\tsuccess\n";
  } else {
    std::cout << "Test 3:\tfailed\n";
  }
}