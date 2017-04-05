#include <iostream>
#include <cstring>
#include "../include/OptParser.h"

int main() {
  /*
   *  Accepted cases:
   *  1 -x
   *  2 -xvalue
   *  3 -x value
   *  4 -x=value
   *  with x defining an argument option and value additional INFOo
   */
  int testSuccess;
  int ret = 0;

  //test cases
  char *case1 = new char[3];
  char *case2 = new char[8];
  char *onlyValue = new char[6];
  char *case4 = new char[9];
  char *filler = new char[5];
  char *dash = new char[2];
  char *case4wrong = new char[4];

  std::strcpy(case1, "-x");
  std::strcpy(case2, "-xvalue");
  std::strcpy(onlyValue, "value");
  std::strcpy(case4, "-x=value");
  std::strcpy(filler, "test");
  std::strcpy(dash, "-");
  std::strcpy(case4wrong, "-x=");

  char *allVals[] = {filler, case1, case2, case1, onlyValue, case4, case1};
  char *c1[] = {filler, case1};
  char *c2[] = {filler, case2};
  char *c3[] = {filler, case1, onlyValue};
  char *c4[] = {filler, case4};
  char *onlyDash[] = {filler, dash};
  char *onlyEquals[] = {filler, case4wrong};
  char *emptyVal[] = {filler};
  char *multiOpt[] = {filler, case1, onlyValue, onlyValue};

  CmdLineOptParser parser;

  testSuccess = parser.Parse(7, allVals) ? 0 : 1;         //should be true
  if(!testSuccess){
    std::cout << "Test 1: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 1: failed\n";
  }

  testSuccess |= parser.Parse(2, c1) ? 0 : 1;             //should be true
  if(!testSuccess){
    std::cout << "Test 2: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 2: failed\n";
  }

  testSuccess |= parser.Parse(2, c2) ? 0 : 1;             //should be true
  if(!testSuccess){
    std::cout << "Test 3: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 3: failed\n";
  }

  testSuccess |= parser.Parse(3, c3) ? 0 : 1;             //should be true
  if(!testSuccess){
    std::cout << "Test 4: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 4: failed\n";
  }

  testSuccess |= parser.Parse(2, c4) ? 0 : 1;             //should be true
  if(!testSuccess){
    std::cout << "Test 5: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 5: failed\n";
  }

  testSuccess |= parser.Parse(2, onlyDash) ? 1 : 0;       //should be false
  if(!testSuccess){
    std::cout << "Test 6: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 6: failed\n";
  }

  testSuccess |= parser.Parse(2, onlyEquals) ? 1 : 0;     //should be false
  if(!testSuccess){
    std::cout << "Test 7: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 7: failed\n";
  }

  testSuccess |= parser.Parse(1, emptyVal) ? 1 : 0;       //should be false
  if(!testSuccess){
    std::cout << "Test 8: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 8: failed\n";
  }

  testSuccess |= parser.Parse(4, multiOpt) ? 1 : 0;       //should be false
  if(!testSuccess){
    std::cout << "Test 9: success\n";
  } else {
    ret |= 1;
    std::cout << "Test 9: failed\n";
  }

  return ret;
}
