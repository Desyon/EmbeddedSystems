#include "../include/Printf.h"
#include <cstdio>

int main(const int argc, const char *argv[]) {
  char buffer[1024];
  unsigned int test = 42;

  printf("%s\n", Printf(buffer, buffer + 1023, "text %d %u %c %s %x %b %%", -42, test, '*', "test string", 0xfeed, 42));
}