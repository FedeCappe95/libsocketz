#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <climits>
#include <iostream>

#ifdef WINDOWS
#include <Ws2tcpip.h>
#ifndef MINGW           //With MSVC2019 we need ip2string.h that is from the Windows SDK Kit
#include <ip2string.h>  //With MinGW everything is in ws2rcpip.h
#endif
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif


using namespace std;


#ifdef WINDOWS
static bool wsaStarted = false;
#endif


#ifdef WINDOWS
void SocketzInternals::startWsaIfNeeded() {
	if(!wsaStarted) {
		WSADATA wsaData;
		int errorCode = WSAStartup(MAKEWORD(2,2), &wsaData);
	    if(errorCode != 0) {
	        throw SocketException(
				std::string("SocketzInternals::startWsaIfNeeded() -> WSAStartup(...) failde, result is ") +
				std::to_string(errorCode) +
				", WSAGetLastError() is " +
				std::to_string(WSAGetLastError())
			);
	    }
	}
}
#endif

in_addr SocketzInternals::ipv4AddressFromString(const std::string& ipString) {
	#ifdef WINDOWS

	//Note: LPSTR stands for char*
	struct sockaddr_storage ss;
	int ssSize = sizeof(ss);
	if(WSAStringToAddress((LPSTR)ipString.c_str(), AF_INET, NULL, (struct sockaddr *)&ss, &ssSize) != 0) {
		throw SocketException("Hostname not found or error during parsing hostname");
	}
	return ((struct sockaddr_in *)&ss)->sin_addr;

	#else

	in_addr address;
	if(inet_pton(AF_INET, ipString.c_str(), &address) <= 0){
        throw SocketException("Hostname not found or error during parsing hostname");
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
		throw SocketException("Hostname not found or error during parsing hostname");
	}
	return ((struct sockaddr_in6 *)&ss)->sin6_addr;

	#else

	in6_addr address;
	if(inet_pton(AF_INET6, ipString.c_str(), &address) <= 0){
        throw SocketException("Hostname not found or error during parsing hostname");
    }
	return address;

	#endif
}

std::string SocketzInternals::ipv4AddressToString(const in_addr* addr) {
    char buf[INET_ADDRSTRLEN];

    #ifdef WINDOWS

	RtlIpv4AddressToString(addr, buf);

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

    RtlIpv6AddressToString(addr, buf);

    #else

    if(inet_ntop(AF_INET6, addr, buf, sizeof(buf)) == 0) {
        //There was an error
        return "unknown";
    }

    #endif


    return string(buf);
}
