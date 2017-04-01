#include <iostream>
#include <cstring>
#include "OptParser.h"

int main(int argc, char *argv[]) {
  /*
   *  Accepted cases:
   *  1 -x
   *  2 -xvalue
   *  3 -x value
   *  4 -x=value
   *  with x defining an argument option and value additional INFOo
   */


   //test cases
   char* case1 = new char[3];
   char* case2 = new char[8];
   char* onlyValue = new char[6];
   char* case4 = new char[9];
   char* filler = new char[5];
   char* dash = new char[2];
   char* case4wrong = new char[4];

   std::strcpy(case1, "-x");
   std::strcpy(case2, "-xvalue");
   std::strcpy(onlyValue, "value");
   std::strcpy(case4, "-x=value");
   std::strcpy(filler, "test");
   std::strcpy(dash, "-");
   std::strcpy(case4wrong, "-x=");

   char* allVals[] = {filler, case1, case2, case1, onlyValue, case4, case1};
   char* c1[] = {filler, case1};
   char* c2[] = {filler, case2};
   char* c3[] = {filler, case1, onlyValue};
   char* c4[] = {filler, case4};
   char* onlyDash[] = {filler, dash};
   char* onlyEquals[] = {filler, case4wrong};
   char *emptyVal[] = {filler};
   char *multiOpt[] = {filler, case1, onlyValue, onlyValue};

  CmdLineOptParser parser;

  std::cout << "Input:\t\t" << parser.Parse(argc, argv) << "\n";        //depends on Input
  std::cout << "allVals:\t" << parser.Parse(7, allVals) << "\n";        //should be true
  std::cout << "Case 1:\t\t" << parser.Parse(2, c1) << "\n";            //should be true
  std::cout << "Case 2:\t\t" << parser.Parse(2, c2) << "\n";            //should be true
  std::cout << "Case 3:\t\t" << parser.Parse(3, c3) << "\n";            //should be true
  std::cout << "Case 4:\t\t" << parser.Parse(2, c4) << "\n";            //should be true
  std::cout << "Only Dash:\t" << parser.Parse(2, onlyDash) << "\n";     //should be false
  std::cout << "Only Equals:\t" << parser.Parse(2, onlyEquals) << "\n"; //should be false
  std::cout << "Empty Vals:\t" << parser.Parse(1, emptyVal) << "\n";    //should be false
  std::cout << "Multi Option:\t" << parser.Parse(4, multiOpt) << "\n";  //should be false
}
