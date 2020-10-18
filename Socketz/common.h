#ifndef __COMMON_H__
#define __COMMON_H__


#ifdef WINDOWS
#pragma comment(lib,"ws2_32.lib")   //winsock2
#pragma comment(lib,"ntdll.lib")    //ip2string.h
#endif


#ifdef WINDOWS
#include <Ws2tcpip.h>
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

    #ifdef WINDOWS
    void startWsaIfNeeded();
    #endif
    in_addr ipv4AddressFromString(const std::string& ipString);
    in6_addr ipv6AddressFromString(const std::string& ipString);
    std::string ipv4AddressToString(const in_addr* addr);
    std::string ipv6AddressToString(const in6_addr* addr);

};

#endif
