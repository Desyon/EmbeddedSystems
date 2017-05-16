#include "../include/Communicator.h"
#include "../include/Endpoint.h"

bool Communicator::Start(Mode mode, unsigned int port, const char *ip) {
  if (Mode::Client == mode) {
    Endpoint clientServer;
    return clientServer.Start(Endpoint::Mode::Client, port, ip);
  } else if (Mode::Server == mode) {
    Endpoint csr;
    return csr.Start(Endpoint::Mode::Server, port, ip);
  } else {
    return false;
  }
}
