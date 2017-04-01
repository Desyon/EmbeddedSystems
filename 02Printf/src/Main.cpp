#include "../include/Printf.h"
#include <cstdio>

int main() {
  char buffer[1024];
  unsigned int test = 42;

  printf("%s\n", Printf(buffer, buffer + 1023, "Forty Two %d %u %c %x %b %%", -42, test, '*', 42, 42));
  //expected result: "Forty Two -42 42 * 0x2a 0b101010 %"

  //printf("%s\n", Printf(buffer, buffer + 1023, "Forty Two %d %u %c %s %x %b %%", -42, test, '*', "Hello World!", 42, 42));
  //expected result: "Forty Two -42 42 * Hello World! 0x2a 0b101010 %"
}