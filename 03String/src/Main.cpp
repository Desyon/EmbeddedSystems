#include <iostream>

#include "../include/PreAllocString.h"
int main() {
  CREATE(string10, 10);
  CREATE(string42, 42);

  /*Testing*/
  /*01 Char Operators*/
  string42 = '4';
  string42 += '2';
  std::cout << "Test 1:\tLength: " << string42.GetLength() << "\tMax: " << string42.SizeOf() << "\t\tContent: " <<
            (const char *) string42 << std::endl;

  /*02 String Operators*/
  string42 = "Forty";
  string42.AddWhiteSpace();
  string42 += "Two";
  std::cout << "Test 3:\tLength: " << string42.GetLength() << "\tMax: " << string42.SizeOf() << "\t\tContent: " <<
            (const char *) string42 << std::endl;

  /*03 Overflow*/
  string10 = "Forty Two and some more content";
  std::cout << "Test 3:\tLength: " << string10.GetLength() << "\tMax: " << string10.SizeOf() << "\t\tContent: " <<
            (const char *) string10 << std::endl;

  /*04 */

}