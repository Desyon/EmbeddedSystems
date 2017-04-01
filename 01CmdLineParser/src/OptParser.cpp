/*
 * created by desyon on 20.03.17
 */
#include "OptParser.h"

#define FLAG '-'
#define END_OF_STRING '\0'

/*
 *  @function Parse
 *  @param argument counter [argc] containing the number of elements in argv
 *  @param argument value [argv] containg the value of all passes arguments
 *  @return bool returns true if all arguments are successfully parsed
 */
bool CmdLineOptParser::Parse (int argc , char * argv[]) {
  /*
   *  Accepted cases:
   *  1 -x
   *  2 -xvalue
   *  3 -x value
   *  4 -x=value
   *  with x defining an argument option and value additional INFOo
   */

  if(2 > argc || !argv){
    return false;
  }

  char argFlag;
  char * argInfo;

  for(int argn = 1; argn < argc; argn++){
    argFlag = 0;
    argInfo = nullptr;

    //check if the passed contains a '-'
    if(FLAG != argv[argn][0]){
     return false;
    }

    //check and set the option of the flag
    if(END_OF_STRING == argv[argn][1]){
       return false;
    }
    argFlag = argv[argn][1];

    //distinguish cases 1 & 3 or 2 & 4
    if(END_OF_STRING != argv[argn][2]){
      //case 2 or 4
      //distinguish cases 2 and 4
      if('=' == argv[argn][2]){
        if(END_OF_STRING == argv[argn][3]){
          return false;
        }
        //case 4
        argInfo = &(argv[argn][3]);
      }
      else {
        //case 2
        argInfo = &(argv[argn][2]);
      }
    }
    else {
      //case 1 or 3
      //distinguish cases 1 and 3
      if((argn + 1) == argc){
        //case 1 as last argument
        argInfo = nullptr;
      }
      else {
        if(FLAG == argv[argn+1][0]){
          //case 1 in the middle of arguments
          argInfo = nullptr;
        }
        else {
          //case 3
          argInfo = &(argv[argn+1][0]);
          //skip next argument in validation loop
          argn++;
        }
      }
    }
    //validate parsed option and info in Option
    if(!Option(argFlag, argInfo)){
      return false;
    }
  }

  return true;
}

/*
 *  @function Option
 *  @param [<const char>] contains the argument option read in Parse
 *  @param [<chons char *>] contains th argument info read in Parse
 *  @return bool returns true if the combination of option and info could be validated
 */
bool CmdLineOptParser::Option(const char, const char *){
  return true;
}
