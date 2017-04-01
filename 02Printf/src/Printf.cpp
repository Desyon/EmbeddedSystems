#include "../include/Printf.h"

#define END_OF_STRING '\0'  //set EoS for nicer code
#define FORMAT_IDENT '%'    //set format input identifier

#define INTEGER 'd'         //set identifier char for integer
#define UNSIGNED 'u'        //set identifier char for unsigned integer
#define CHARACTER 'c'       //set identifier char for characters
#define STRING 's'          //set identifier char for strings
#define HEXADECIMAL 'x'     //set identifier char for hex numbers
#define BINARY 'b'          //set identifier char for binary numbers
#define PERCENT '%'         //set identifier char for percent signs

char *numberToBase(char *, const void *, unsigned int, unsigned int);
static const char digits[] = "0123456789abcdef";

char *Printf(char *destination, const void *end, const char *format, ...) {
  //check if any argument is empty
  if (!destination || !end || !format) {
    return nullptr;
  }

  va_list argList;
  va_start(argList, format);

  const char *position = format; //position counter in the format string
  char *nextOutput = destination;

  for (position; END_OF_STRING != *position; position++) {
    //check if formatted print is needed
    if (FORMAT_IDENT != *position) {

    } else {
      char formatChar = *(position + 1);
      switch (formatChar) {
        case INTEGER: break;
        case UNSIGNED: {
          nextOutput = numberToBase(nextOutput, end, va_arg(argList, unsigned int), 10);
          nextOutput++;
          break;
        }
        case CHARACTER: {
          *nextOutput = va_arg(argList, int);
          nextOutput++;
          break;
        }
        case STRING: break;
        case HEXADECIMAL: break;
        case BINARY: break;
        case PERCENT: break;
        default: return nullptr; //if unrecognized return nullptr
      }
    }
  }
}

char *numberToBase(char *output, const void *end, unsigned int value, unsigned int base) {
  //check if an argument is empty or the output equal or behind the end of the output buffer
  if(!output || !end || output >= end){
    return nullptr;
  }

  char digit = digits[value % base];    //get char from array "digits"
  value = value / base;                 //shift the value by the amount of base

  //recursion if the there are still digits left
  if(value){
    numberToBase(output, end, value, base);
  }

  *output = digit;    //write the digit
  return ++output;    //increase and return the pointer
}

