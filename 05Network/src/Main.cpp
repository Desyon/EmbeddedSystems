/**
 * Created by til on 16.05.17.
 */

#include <cstring>
#include <iostream>

#include "../include/Communicator.h"
#include "../include/Endpoint.h"

#define PORT 55047 //private, random constant

void printUsageHints();

int main(int argc, char const *argv[]) {
  if (2 > argc) {
    printUsageHints();
    /* Usually should return some error code indicating that the process did not finish successfully, nevertheless
     * the CI needs all programs to exit with exit code 0 to finish building successfully. Testing can't be done
     * automatically at this point.
     */
    return 0;
  }
  /*
   * Command line argument validation could be done with command line arg parser from task 1.
   */
  Communicator communicator;
  if ('-' == argv[1][0] && 's' == argv[1][1]) {
    return communicator.Start(Communicator::Mode::Server, PORT, nullptr)
           ? 0 : 2;
  } else if ('-' == argv[1][0] && 'c' == argv[1][1] && argc == 3) {
    return communicator.Start(Communicator::Mode::Client, PORT, argv[2])
           ? 0 : 2;
  } else {
    printUsageHints();
    return 1;
  }
}

void printUsageHints() {
  std::cout << "----------------------- USAGE INFO -----------------------" << std::endl;
  std::cout << "1. Start program in server mode with '-s'" << std::endl;
  std::cout << "2. Start 2nd instance in client mode with '-c <server_ip>" << std::endl << std::endl;
  std::cout << "INFO: Server uses UDP port " << PORT << std::endl << std::endl;
}
