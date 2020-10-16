#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <climits>
#include <iostream>

#ifdef WINDOWS
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#endif

using namespace std;

in_addr SocketzInternals::ipv4AddressFromString(const std::string& ipString) {
	#ifdef WINDOWS

	//Note: LPSTR stands for char*
	struct sockaddr_storage ss;
	int ssSize = sizeof(ss);
	if(WSAStringToAddress((LPSTR)ipString.c_str(), AF_INET, NULL, (struct sockaddr *)&ss, &ssSize) != 0) {
		throw SocketError("Hostname not found or error during parsing hostname");
	}
	return ((struct sockaddr_in *)&ss)->sin_addr;

	#else

	in_addr address;
	if(inet_pton(AF_INET, ipString.c_str(), &address) <= 0){
        throw SocketError("Hostname not found or error during parsing hostname");
    }
	return address;

	#endif
}

in6_addr SocketzInternals::ipv6AddressFromString(const std::string& ipString) {
	#ifdef WINDOWS

	//Note: LPSTR stands for char*
	struct sockaddr_storage ss;
	int ssSize = sizeof(ss);
	if(WSAStringToAddress((LPSTR)ipString.c_str(), AF_INET6, NULL, (struct sockaddr *)&ss, &ssSize) != 0) {
		throw SocketError("Hostname not found or error during parsing hostname");
	}
	return ((struct sockaddr_in6 *)&ss)->sin6_addr;

	#else

	in6_addr address;
	if(inet_pton(AF_INET6, ipString.c_str(), &address) <= 0){
        throw SocketError("Hostname not found or error during parsing hostname");
    }
	return address;

	#endif
}

std::string SocketzInternals::ipv4AddressToString(const in_addr* addr) {
    char buf[INET_ADDRSTRLEN];

    #ifdef WINDOWS

    struct sockaddr_storage ss;
    unsigned long s = sizeof(buf);
    ZeroMemory(&ss, sizeof(ss));
    ss.ss_family = AF_INET;
    ((struct sockaddr_in *)&ss)->sin_addr = *addr;

    if(WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, buf, &s) == 0) {
        //There was an error
        return string("unknown");
    }

    #else

    if(inet_ntop(AF_INET, addr, buf, sizeof(buf)) == 0) {
        //There was an error
        return string("unknown");
    }

    #endif

    return string(buf);
}

std::string SocketzInternals::ipv6AddressToString(const in6_addr* addr) {
    char buf[INET6_ADDRSTRLEN];

    #ifdef WINDOWS

    struct sockaddr_storage ss;
    unsigned long s = sizeof(buf);
    ZeroMemory(&ss, sizeof(ss));
    ss.ss_family = AF_INET6;
    ((struct sockaddr_in6 *)&ss)->sin6_addr = *addr;

    if(WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, buf, &s) == 0) {
        //There was an error
        return "unknown";
    }

    #else

    if(inet_ntop(AF_INET6, addr, buf, sizeof(buf)) == 0) {
        //There was an error
        return "unknown";
    }

    #endif


    return string(buf);
}
