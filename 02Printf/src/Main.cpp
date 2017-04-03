#include "../include/Printf.h"
#include <iostream>

int main() {
  char buffer[1024];
  char tinyBuffer[10];

  unsigned int test = 42;

  int testSuccess = 0;

  //testing
  testSuccess = Printf(buffer, buffer + 1023, "Forty Two %d %u %c %s %x %b %%", -42, test, '*', "Forty Two", 42, 42) ? 0 : 1;
  std::cout << buffer << "\n";
  buffer[0] = '\0';
  testSuccess |= Printf(tinyBuffer, tinyBuffer + 9, "A String That is too long for the buffer") ? 1 : 0;
  std::cout << buffer << "\n";
  buffer[0] = '\0';
  testSuccess |= Printf(buffer, buffer + 1023, "%w %%%") ? 0 : 1;
  std::cout << buffer << "\n";

  //if any test failed testSuccess is not 0
  return testSuccess;
}