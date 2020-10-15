#ifndef __COMMON_H__
#define __COMMON_H__


#ifdef WINDOWS
#include <Ws2tcpip.h>
#include <Windows.h>
#else
#include <netinet/in.h>
#endif

#include <string>
#include "SocketError.h"

#ifdef WINDOWS
typedef SOCKET SocketDescriptor;
#else
typedef int SocketDescriptor;
#endif

typedef unsigned char byte;

enum InternetProtocol {IPv4, IPv6};

// Help-functions usefull in some classes
namespace SocketzInternals {

    in_addr ipv4AddressFromString(const std::string& ipString);
    in6_addr ipv6AddressFromString(const std::string& ipString);
    std::string ipv4AddressToString(const in_addr* addr);
    std::string ipv6AddressToString(const in6_addr* addr);

};

#endif
