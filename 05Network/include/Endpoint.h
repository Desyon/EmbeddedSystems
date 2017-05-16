#ifndef CLIENTSERVERREAL_H
#define CLIENTSERVERREAL_H

#include <cstdint> /* for uintXX_T */
#include <cstring> /* for memcpy, memset */
#include <sys/types.h> /* for socket_addr */
#include <sys/socket.h> /* for socket */
#include <netinet/in.h> /* for socket type */
#include <arpa/inet.h> /* for socket addr */
#include <netdb.h> /* for gethostbyname */
#include <unistd.h> /* for close and write*/

#include "PoolAllocator.h"

/**
 * Endpoint class making up server as well as the client in the current state of the project. Communication only is
 * roughly tested with some dummy packets that are send from the client to the server. The server acknowledges each
 * packet.
 */
class Endpoint {
 public:
  enum Mode : unsigned int {
    Server,
    Client
  };

 public:
  /**
   * struct for the packets that are send. Contains a field for payload length as well as a sequence number.
   * The payload is written to a pointer of an array of size 0.
   */
  struct Packet {
    uint16_t payloadLength;
    uint16_t sequenceNumber;
    uint16_t command;
    uint16_t handle;
    uint8_t data[0];
  } __attribute__((packed));

  /**
   * Commands supported by the protocol.
   */
  enum Command : uint16_t {
    Invalid = 0,
    GeneralErrorReply = 1,
    Echo = 2,
    EchoReply = 3,
    Shutdown = 4,
    ShutdownReply = 5,
    Unsupported = 6
  };

  /**
   * Wrapper for packets that adds checksums to ensure correct packet transfer and enable easy acknowledging.
   * This struct is actually send between client and server.
   */
  struct Wrapper {
    uint16_t totalLength;
    uint16_t sequenceNumber;
    uint16_t ackNumber;
    uint16_t packetChecksum;
    uint8_t type;
    Packet packet[0];
  } __attribute__((packed));

  /**
   * Gives the type of send wrapper unit (that is a network packet). Available values are ACK, NACK and DATA for
   * respective purposes.
   */
  enum WrapperType : uint8_t {
    ACK = 0,
    NACK = 1,
    DATA = 2
  };

 public:
  /* usefull for UDP datagramm */
  static constexpr uint16_t MAX_TRANSMISSION_LENGTH = 1500;
  static constexpr uint16_t MAX_PACKET_LENGTH =
      MAX_TRANSMISSION_LENGTH - sizeof(Wrapper);
  static constexpr uint16_t MAX_PAYLOAD_LENGTH =
      MAX_PACKET_LENGTH - sizeof(Packet);
  static constexpr size_t MAX_RETRIES = 10;

 public:
  Endpoint()
      : packetBuffer(),
        sequenceNumber(1),
        comSocket(-1),
        myAddr(),
        otherAddr(),
        m_mode(Mode::Server),
        wrapperNumber(1) {}

  bool Start(Mode, unsigned int, const char *);

 private:
  bool StartServer(unsigned int, const char *);
  bool StartClient(unsigned int, const char *);

  bool InitServer(unsigned int, const char *);
  bool InitClient(unsigned int, const char *);

  bool ShutdownServer();
  bool ShutdownClient();

  bool ServerCreateResponse(Packet *, Packet *);

  bool CreatePacket(Packet *, uint16_t, uint16_t, Command, uint16_t, const void *);

  bool SendPacket(const Packet *packet);

  /* blocking */
  bool RecvPacket(Packet *buffer);
  void HostToNetPacket(Packet *packet);
  void NetToHostPacket(Packet *packet);

  uint16_t NextSequenceNumber();

  uint16_t PacketChecksum(const Packet *);

  bool ValidateWrapper(const Wrapper *, ssize_t, uint16_t);

  bool SendAckWrapper(uint16_t, uint16_t);
  bool SendNackWrapper(uint16_t, uint16_t);
  bool SendDataWrapper(const Packet *, uint16_t);

  /* blocking and validating */
  bool RecvWrapper(Wrapper *, uint16_t);

  void HostToNetWrapper(Wrapper *);
  void NetToHostWrapper(Wrapper *);

  uint16_t NextWrapperNumber();

 private:
  PoolAllocator<10, MAX_PACKET_LENGTH> packetBuffer;
  uint16_t sequenceNumber;
  int comSocket;
  sockaddr_in myAddr, otherAddr;
  Mode m_mode;
  uint16_t wrapperNumber;
};

/* fletcher 16 checksum */
uint16_t fletcher_16(const uint8_t *, const size_t);

#endif /* CLIENTSERVERREAL_H */
