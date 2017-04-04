#include "../include/Printf.h"
#include <iostream>

int main() {
  char buffer[1024];
  char tinyBuffer[10];

  unsigned int test = 42;

  int testSuccess;
  int ret = 0;

  //testing
  testSuccess = Printf(buffer, buffer + 1023,
                       "Forty Two %d %u %c %s %x %b %%", -42, test, '*', "Forty Two", 42, 42) ? 1 : 0;
  if(testSuccess){
    std::cout << "Test 1: success:\t" << buffer << "\n";
  } else {
    ret = 1;
    std::cout << "Test 1: failed\n";
  }

  buffer[0] = '\0';
  testSuccess = Printf(buffer, buffer + 1023, "%w %%%") ? 1 : 0;
  if(testSuccess){
    std::cout << "Test 2: success:\t" << buffer << "\n";
  } else {
    ret |= 1;
    std::cout << "Test 2: failed\n";
  }

  //buffer overflow
  tinyBuffer[0] = '\0';
  testSuccess = Printf(tinyBuffer, tinyBuffer + 9, "A String That is too long for the buffer") ? 1 : 0;
  if(testSuccess){
    std::cout << "Test 3: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 3: failed\n";
  }

  //buffer overflow with numbers
  tinyBuffer[0] = '\0';
  testSuccess = Printf(tinyBuffer, tinyBuffer + 9, "%b", 250000000) ? 1 : 0;
  if(testSuccess){
    std::cout << "Test 4: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 4: failed\n";
  }

  //null argument
  tinyBuffer[0] = '\0';
  testSuccess = Printf(tinyBuffer, tinyBuffer + 9, 0, 25000) ? 0 : 1;
  if(testSuccess){
    std::cout << "Test 5: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 5: failed\n";
  }

  //empty format string
  tinyBuffer[0] = '\0';
  testSuccess = Printf(tinyBuffer, tinyBuffer + 9, "") ? 1 : 0;
  std::cout << tinyBuffer;
  if(testSuccess){
    std::cout << "Test 6: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 6: failed\n";
  }

  //if any test failed testSuccess is not 0
  return ret;
}