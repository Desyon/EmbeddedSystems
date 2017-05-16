#include <iostream>
#include "../include/Endpoint.h"

bool Endpoint::Start(Mode mode, unsigned int port, const char *ip) {
  if (Mode::Client == mode
      && !ip) {
    return false;
  } /* ip may be null for server when binding to all interfaces */
  m_mode = mode;
  if (Mode::Server == mode) {
    return StartServer(port, ip);
  } else if (Mode::Client == mode) {
    return StartClient(port, ip);
  } else {
    return false;
  }
}

bool Endpoint::StartServer(unsigned int port, const char *ip) {
  std::cout << "INFO:\tStarting mode=Server, port=" << port;
  if (!ip) {
    std::cout << std::endl;
  } else {
    std::cout << ", ip=" << ip << std::endl;
  }

  if (!InitServer(port, ip)) {
    std::cout << "ERROR:\tFailed to init server" << std::endl;
    return false;
  }

  bool running = true;
  Packet *recvBuf =
      static_cast<Packet *>(packetBuffer.Allocate(MAX_PACKET_LENGTH));
  Packet *sendBuf =
      static_cast<Packet *>(packetBuffer.Allocate(MAX_PACKET_LENGTH));

  std::cout << "INFO:\tServer running" << std::endl;

  while (running) {
    memset(recvBuf, 0, MAX_PACKET_LENGTH);
    memset(sendBuf, 0, MAX_PACKET_LENGTH);
    /* blocking */
    if (!RecvPacket(recvBuf)) {
      std::cout << "ERROR:\tFailed to recieve packet" << std::endl;
      continue;
    }
    //std::cout << "Server recieved packet:" << std::endl;
    //PrintPacketInfo(recvBuf);
    if (!ServerCreateResponse(sendBuf, recvBuf)) {
      std::cout << "ERROR:\tFailed to create response" << std::endl;
      continue;
    }
    //std::cout << "Server sending packet:" << std::endl;
    //PrintPacketInfo(sendBuf);
    if (!SendPacket(sendBuf)) {
      std::cout << "ERROR:\tFailed to send response" << std::endl;
      continue;
    }
    if (Command::Shutdown == recvBuf->command) {
      //std::cout << "Recieved Command::Shutdown, shutting down" << std::endl;
      if (!ShutdownServer()) {
        std::cout << "ERROR:\tFailed to shut down server" << std::endl;
        return false; /* actually return here */
      }
      running = false;
    }
  }

  packetBuffer.Deallocate(recvBuf);
  packetBuffer.Deallocate(sendBuf);

  std::cout << "INFO:\tServer shut down." << std::endl;

  return true;
}

bool Endpoint::StartClient(unsigned int port, const char *ip) {
  std::cout << "INFO:\tStarting mode=Client, port=" << port << ", ip=" << ip << std::endl;

  if (!InitClient(port, ip)) {
    std::cout << "ERROR:\tFailed to init client" << std::endl;
    return false;
  }

  Packet *recvBuf =
      static_cast<Packet *>(packetBuffer.Allocate(MAX_PACKET_LENGTH));
  Packet *sendBuf =
      static_cast<Packet *>(packetBuffer.Allocate(MAX_PACKET_LENGTH));
  memset(recvBuf, 0, MAX_PACKET_LENGTH);
  memset(sendBuf, 0, MAX_PACKET_LENGTH);

  bool running = true;

  std::cout << "INFO:\tClient running" << std::endl;

  /* arbitrary test commands */

  memset(sendBuf, 0, MAX_PACKET_LENGTH);
  memset(recvBuf, 0, MAX_PACKET_LENGTH);
  char data[] = "TEST";
  CreatePacket(sendBuf, sizeof(data), NextSequenceNumber(), Command::Echo, 11, data);
  //std::cout << "Sending:" << std::endl;
  //PrintPacketInfo(sendBuf);
  SendPacket(sendBuf);
  RecvPacket(recvBuf);
  //std::cout << "Recieved:" << std::endl;
  //PrintPacketInfo(recvBuf);

  memset(sendBuf, 0, MAX_PACKET_LENGTH);
  memset(recvBuf, 0, MAX_PACKET_LENGTH);
  CreatePacket(sendBuf, 0, NextSequenceNumber(), Command::Invalid, 22, nullptr);
  //std::cout << "Sending:" << std::endl;
  //PrintPacketInfo(sendBuf);
  SendPacket(sendBuf);
  RecvPacket(recvBuf);
  //std::cout << "Recieved:" << std::endl;
  //PrintPacketInfo(recvBuf);

  memset(sendBuf, 0, MAX_PACKET_LENGTH);
  memset(recvBuf, 0, MAX_PACKET_LENGTH);
  CreatePacket(sendBuf, 0, NextSequenceNumber(), Command::Unsupported, 33, nullptr);
  //std::cout << "Sending:" << std::endl;
  //PrintPacketInfo(sendBuf);
  SendPacket(sendBuf);
  RecvPacket(recvBuf);
  //std::cout << "Recieved:" << std::endl;
  //PrintPacketInfo(recvBuf);

  /* ending with server shutdown */
  uint16_t handle = 66;
  while (running) {
    memset(sendBuf, 0, MAX_PACKET_LENGTH);
    memset(recvBuf, 0, MAX_PACKET_LENGTH);
    CreatePacket(sendBuf, 0, NextSequenceNumber(), Command::Shutdown, handle, nullptr);
    //std::cout << "Sending:" << std::endl;
    //PrintPacketInfo(sendBuf);
    SendPacket(sendBuf);
    RecvPacket(recvBuf);
    //std::cout << "Recieved:" << std::endl;
    //PrintPacketInfo(recvBuf);
    if (sendBuf->handle == recvBuf->handle
        && static_cast<uint16_t>(Command::ShutdownReply) == recvBuf->command) {
      //std::cout << "Recieved ShutdownReply" << std::endl;
      running = false;
    } else {
      //std::cout << "Did not recieve ShutdownReply" << std::endl;
    }
    handle++;
  }

  if (!ShutdownClient()) {
    std::cout << "ERROR:\tFailed to shut down client" << std::endl;
    return false;
  }

  packetBuffer.Deallocate(recvBuf);
  packetBuffer.Deallocate(sendBuf);

  std::cout << "INFO:\tClient shut down." << std::endl;

  return true;
}

bool Endpoint::InitServer(unsigned int port, const char *ip) {
  /* ip may be null for INADDR_ANY */
  std::cout << "INFO:\tInitializing server" << std::endl;
  if (!port) {
    return false;
  }
  /* create socket */
  comSocket = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (comSocket < 0) {
    std::cout << "ERROR:\tFailed to open socket" << std::endl;
    return false;
  }
  // Bind server to given port and ip or any ip if nullprt
  // we dont need other_addr here
  // step 1 find out the server address we are on
  memset((char *) &myAddr, 0, sizeof(myAddr));
  myAddr.sin_family = AF_INET;
  myAddr.sin_port = htons(port);
  if (ip) {
    hostent *hp; /* host information */
    hp = gethostbyname(ip);
    if (hp) {
      memcpy((void *) &(myAddr.sin_addr), hp->h_addr_list[0], hp->h_length);
      /* just pick the first one here, TODO validate that its ipv4 */
    } else {
      //std::cout << "Could not get host for " << ip << "fallback to INADDR_ANY" << std::endl;
      myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
  } else {
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  }

  /* step 2 do the bind to server address */
  return bind(comSocket, reinterpret_cast<sockaddr *>(&myAddr), sizeof(myAddr)) >= 0;

  /* all went well */
}

bool Endpoint::InitClient(unsigned int port, const char *ip) {
  std::cout << "INFO:\tInitializing client" << std::endl;
  if (!ip || !port) {
    return false;
  }
  /* create socket */
  comSocket = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (comSocket < 0) {
    std::cout << "INFO:\tFailed to open socket" << std::endl;
    return false;
  }

  /* find our address to be any, this is where we bind the socket to */
  memset((char *) &myAddr, 0, sizeof(myAddr));
  myAddr.sin_family = AF_INET;
  myAddr.sin_port = htons(0); /* any port */
  myAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* any address */

  /* find out server address, this is where we send messages */
  memset((char *) &otherAddr, 0, sizeof(otherAddr));
  otherAddr.sin_family = AF_INET;
  otherAddr.sin_port = htons(static_cast<uint16_t>(port));
  hostent *hp; /* host information */
  hp = gethostbyname(ip);
  if (hp) {
    memcpy((void *) &(otherAddr.sin_addr), hp->h_addr_list[0], static_cast<size_t>(hp->h_length));
    /* just pick the first one here, TODO validate that its ipv4 */
  } else {
    //std::cout << "Could not get host for " << ip << std::endl;
    return false;
  }

  /* bind to client addr */
  return 0 <= bind(comSocket, reinterpret_cast<sockaddr *>(&myAddr), sizeof(myAddr));

  /* all went fine */
}

bool Endpoint::ShutdownServer() {
  std::cout << "INFO:\tShutting down server" << std::endl;
  close(comSocket);
  return true;
}

bool Endpoint::ShutdownClient() {
  std::cout << "INFO:\tShutting down client" << std::endl;
  close(comSocket);
  return true;
}

bool Endpoint::ServerCreateResponse(Packet *buffer, Packet *received) {
  if (!buffer
      || !received) {
    return false;
  }
  switch (static_cast<Command>(received->command)) {
    case Command::Echo : {
      return CreatePacket(buffer, received->payloadLength,
                          NextSequenceNumber(), Command::EchoReply,
                          received->handle, received->data);
    }
      break;
    case Command::Shutdown : {
      return CreatePacket(buffer, 0, NextSequenceNumber(),
                          Command::ShutdownReply, received->handle, nullptr);
    }
      break;
    default: {
      uint16_t data[1] = {received->command};
      return CreatePacket(buffer, sizeof(data), NextSequenceNumber(),
                          Command::GeneralErrorReply, received->handle,
                          data);
    }
      break;
  }
}

bool Endpoint::CreatePacket(Packet *buffer, uint16_t payloadLength,
                            uint16_t sequenceNumber, Command command,
                            uint16_t handle, const void *dataBuf) {
  if (!buffer
      || payloadLength > MAX_PAYLOAD_LENGTH) {
    return false;
  }
  buffer->payloadLength = payloadLength;
  buffer->sequenceNumber = sequenceNumber;
  buffer->command = static_cast<uint16_t>(command);
  buffer->handle = handle;
  if (dataBuf) {
    std::memcpy(buffer->data, dataBuf, payloadLength);
  }
  return true;
}

bool Endpoint::SendPacket(const Packet *packet) {
  if (!packet) {
    return false;
  }
  uint8_t buf[MAX_TRANSMISSION_LENGTH];
  Wrapper *recvBuf = reinterpret_cast<Wrapper *>(buf);
  for (size_t retries = 0; retries < MAX_SEND_RETRIES; retries++) {
    uint16_t sendSeq = NextWrapperNumber();
    if (!SendDataWrapper(packet, sendSeq)) {
      std::cout << "WARN:\tFailed to send Wrapper, retrying " << retries + 1 << "/" << MAX_SEND_RETRIES << std::endl;
      //usleep(10);
      continue;
    }
    if (!RecvWrapper(recvBuf, sendSeq)
        || static_cast<uint16_t>(WrapperType::ACK) != recvBuf->type) {
      std::cout << "WARN:\tFailed to recieve Wrapper, retrying " << retries + 1 << "/" << MAX_SEND_RETRIES << std::endl;
      //usleep(10 << std::endl;
      continue;
    }
    /* went well */
    return true;
  }
  /* looped to long, failure */
  return false;
}

/* blocking */
bool Endpoint::RecvPacket(Packet *buffer) {
  if (!buffer) {
    return false;
  }
  uint8_t buf[MAX_TRANSMISSION_LENGTH];
  Wrapper *recvBuf = reinterpret_cast<Wrapper *>(buf);
  if (!RecvWrapper(recvBuf, 0)
      || static_cast<uint16_t>(WrapperType::DATA) != recvBuf->type) {
    if (!SendNackWrapper(recvBuf->sequenceNumber, NextWrapperNumber())) {
      return false;
    }
  } else {
    if (!SendAckWrapper(recvBuf->sequenceNumber, NextWrapperNumber())) {
      return false;
    }
  }
  memcpy(buffer, recvBuf->packet, recvBuf->packet->payloadLength + sizeof(Packet));
  /* all went well */
  return true;
}

/* will not convert payload */
void Endpoint::HostToNetPacket(Packet *packet) {
  if (!packet) {
    return;
  }
  /* htons converts host to net uint16_t */
  packet->payloadLength = htons(packet->payloadLength);
  packet->sequenceNumber = htons(packet->sequenceNumber);
  packet->command = htons(packet->command);
  packet->handle = htons(packet->handle);
}

/* will not convert payload */
void Endpoint::NetToHostPacket(Packet *packet) {
  if (!packet) {
    return;
  }
  /* htons converts net to host uint16_t */
  packet->payloadLength = ntohs(packet->payloadLength);
  packet->sequenceNumber = ntohs(packet->sequenceNumber);
  packet->command = ntohs(packet->command);
  packet->handle = ntohs(packet->handle);
}

uint16_t Endpoint::NextSequenceNumber() {
  return sequenceNumber++;
}

uint16_t Endpoint::PacketChecksum(const Packet *packet) {
  if (!packet) {
    return 0;
  }
  return fletcher_16(reinterpret_cast<const uint8_t *>(packet),
                     sizeof(Packet) + packet->payloadLength);
}

bool Endpoint::ValidateWrapper(const Wrapper *wrapper, ssize_t bytes_recieved,
                               uint16_t expectedAckNumber) {
  //std::cout << "Validating wrapper:" << std::endl;
  //PrintWrapperInfo(wrapper);
  if (!wrapper) {
    return false;
  }
  if (bytes_recieved < static_cast<ssize_t>(sizeof(Wrapper))) {
    //std::cout << "To few bytes recieved" << std::endl;
    return false;
  }
  if (wrapper->totalLength != bytes_recieved) {
    //std::cout << "Bytes recieved not matching totalLength" << std::endl;
    return false;
  }
  switch (wrapper->type) {
    case WrapperType::ACK: /* fallthrough */
    case WrapperType::NACK: {
      //std::cout << "Validating for ACK / NACK" << std::endl;
      if (wrapper->totalLength != sizeof(Wrapper)) {
        //std::cout << "Incorrect length" << std::endl;
        return false;
      }
      if (wrapper->ackNumber != expectedAckNumber) {
        //std::cout << "Incorrect ackNumber" << std::endl;
        return false;
      }
      if (wrapper->packetChecksum != 0) {
        //std::cout << "Wrong checksum" << std::endl;
        return false;
      }
    }
      break;
    case WrapperType::DATA: {
      //std::cout << "Validating for data" << std::endl;
      if (wrapper->totalLength !=
          sizeof(Wrapper) + sizeof(Packet) + wrapper->packet->payloadLength) {
        //std::cout << "Incorrect totalLength" << std::endl;
        return false;
      }
      if (wrapper->ackNumber != 0) {
        //std::cout << "Wrong ackNumber" << std::endl;
        return false;
      }
      if (wrapper->packetChecksum != PacketChecksum(wrapper->packet)) {
        //std::cout << "Incorrect checksum compare" << std::endl;
        return false;
      }
    }
      break;
    default: {
      //std::cout << "Invalid type" << std::endl;
      return false;
    }
  }
  //std::cout << "Valid packet" << std::endl;
  return true;
}

bool Endpoint::SendAckWrapper(uint16_t received, uint16_t seqNumber) {
  Wrapper wrapper;
  wrapper.totalLength = sizeof(Wrapper);
  wrapper.sequenceNumber = seqNumber;
  wrapper.ackNumber = received;
  wrapper.packetChecksum = 0;
  wrapper.type = static_cast<uint16_t>(WrapperType::ACK);
  auto size = wrapper.totalLength;
  //std::cout << "sending wrapper:" << std::endl;
  //PrintWrapperInfo(&wrapper);
  HostToNetWrapper(&wrapper);
  return sendto(comSocket, &wrapper, size, 0,
                reinterpret_cast<sockaddr *>(&otherAddr), sizeof(otherAddr)) >= 0;
}

bool Endpoint::SendNackWrapper(uint16_t receivedSeq, uint16_t seqNumber) {
  Wrapper wrapper;
  wrapper.totalLength = sizeof(Wrapper);
  wrapper.sequenceNumber = seqNumber;
  wrapper.ackNumber = receivedSeq;
  wrapper.packetChecksum = 0;
  wrapper.type = static_cast<uint16_t>(WrapperType::NACK);
  auto size = wrapper.totalLength;
  //std::cout << "sending wrapper:" << std::endl;
  //PrintWrapperInfo(&wrapper);
  HostToNetWrapper(&wrapper);
  return sendto(comSocket, &wrapper, size, 0,
                reinterpret_cast<sockaddr *>(&otherAddr), sizeof(otherAddr)) >= 0;
}

bool Endpoint::SendDataWrapper(const Packet *packet, uint16_t seqNumber) {
  if (!packet) {
    return false;
  }
  uint8_t buffer[MAX_TRANSMISSION_LENGTH] = {0};
  Wrapper *wrapper = reinterpret_cast<Wrapper *>(buffer);
  wrapper->totalLength = sizeof(Wrapper) + sizeof(Packet) + packet->payloadLength;
  wrapper->sequenceNumber = seqNumber;
  wrapper->ackNumber = 0;
  wrapper->packetChecksum = PacketChecksum(packet);
  wrapper->type = static_cast<uint16_t>(WrapperType::DATA);
  memcpy(wrapper->packet, packet, sizeof(Packet) + packet->payloadLength);
  auto size = wrapper->totalLength;
  //std::cout << "sending wrapper:" << std::endl;
  //PrintWrapperInfo(wrapper);
  HostToNetPacket(wrapper->packet);
  HostToNetWrapper(wrapper);
  return sendto(comSocket, wrapper, size, 0,
                reinterpret_cast<sockaddr *>(&otherAddr), sizeof(otherAddr)) >= 0;
}

/* blocking and validating */
bool Endpoint::RecvWrapper(Wrapper *buffer, uint16_t expectedAckNumber) {
  if (!buffer) {
    return false;
  }
  sockaddr_in recvAddr;
  socklen_t recvAddrLength = sizeof(recvAddr);
  auto receivedBytes = recvfrom(comSocket, buffer, MAX_TRANSMISSION_LENGTH, 0,
                                reinterpret_cast<sockaddr *>(&recvAddr),
                                &recvAddrLength);
  NetToHostWrapper(buffer);
  if (static_cast<uint16_t>(WrapperType::DATA) == buffer->type) {
    //std::cout << "Converting Net to Host packet" << std::endl;
    NetToHostPacket(buffer->packet);
  }
  //std::cout << "recieved wrapper:" << std::endl;
  //PrintWrapperInfo(buffer);
  if (Mode::Server == m_mode) {
    /* hopefully we only talk to one person */
    memcpy(&otherAddr, &recvAddr, sizeof(recvAddr));
  }
  return ValidateWrapper(buffer, receivedBytes, expectedAckNumber);
}

/* will not convert possible packet */
void Endpoint::HostToNetWrapper(Wrapper *wrapper) {
  if (!wrapper) {
    return;
  }
  wrapper->totalLength = htons(wrapper->totalLength);
  wrapper->sequenceNumber = htons(wrapper->sequenceNumber);
  wrapper->ackNumber = htons(wrapper->ackNumber);
  wrapper->packetChecksum = htons(wrapper->packetChecksum);
  /* type must not be reordered */
}

/* will not convert possible packet */
void Endpoint::NetToHostWrapper(Wrapper *wrapper) {
  if (!wrapper) {
    return;
  }
  wrapper->totalLength = ntohs(wrapper->totalLength);
  wrapper->sequenceNumber = ntohs(wrapper->sequenceNumber);
  wrapper->ackNumber = ntohs(wrapper->ackNumber);
  wrapper->packetChecksum = ntohs(wrapper->packetChecksum);
  /* type must not be reordered */
}

uint16_t Endpoint::NextWrapperNumber() {
  return wrapperNumber++;
}

uint16_t fletcher_16(const uint8_t *data, const size_t length) {
  uint16_t sum1 = 0, sum2 = 0;
  for (size_t i = 0; i < length; i++) {
    sum1 = (sum1 + data[i]) % 255;
    sum2 = (sum2 + sum1) % 255;
  }
  return (sum2 << 8) | sum1;
}
