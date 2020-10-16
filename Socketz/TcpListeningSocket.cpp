#include "TcpListeningSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <climits>
#include <string>
#include <iostream>
#include "SocketError.h"
#include "common.h"

#ifdef WINDOWS
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#endif


TcpListeningSocket::TcpListeningSocket(const InternetProtocol internetProtocol) :
	sockfd(0),
	listening(false)
{
	sin_family = (internetProtocol == IPv4 ? AF_INET : AF_INET6);
}

//Do not close the socket in the destructor!! If you close it here you will
//spend the entire day looking for bugs... Just like I did...
//When you copy a TcpSocket/TcpListeningSocket, the old one will have an invalid
//socketfd as a private data member!
TcpListeningSocket::~TcpListeningSocket() {}


bool TcpListeningSocket::listenTo(const uint16_t port, const uint16_t backlog){
	if(listening)
		throw SocketError("TcpListeningSocket::listenTo(...) -> socket already in listen state");

	int errorCode;

	#ifdef WINDOWS

	//I'm going to copy all the procedure from the Microsoft offcial documentation
	SocketzInternals::startWsaIfNeeded();
	struct addrinfo *result = NULL;
    struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = this->sin_family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
	// Resolve the server address and port
	std::string portString = std::to_string(port);
    errorCode = getaddrinfo(NULL, portString.c_str(), &hints, &result);
    if(errorCode != 0) {
		throw SocketError("TcpListeningSocket::listenTo(...) -> getaddrinfo failed, errorCode is " + errorCode);
    }

	sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(sockfd == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw SocketError("TcpListeningSocket::listenTo(...) -> socket(...) failed, sockfd is " + sockfd);
    }

	errorCode = bind(sockfd, result->ai_addr, (int)result->ai_addrlen);
    if(errorCode == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(sockfd);
        WSACleanup();
        throw SocketError("TcpListeningSocket::listenTo(...) -> bind(...) failed, errorCode is " + errorCode);
    }

    freeaddrinfo(result);

    errorCode = listen(sockfd, backlog);
    if(errorCode == SOCKET_ERROR) {
        closesocket(sockfd);
        WSACleanup();
        throw SocketError(
			std::string("TcpListeningSocket::listenTo(...) -> listen(...) failed, errorCode is ") +
			std::to_string(errorCode) +
			"WSAGetLastError() is " +
			std::to_string(WSAGetLastError())
		);
    }

	//After some tests, it seems I don't need this settings on Windows...
	//const BOOL yes = 1;
	//if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) < 0)
		//throw SocketError("setsockopt(SO_REUSEADDR) failed");

	#else

	sockfd = socket(sin_family, SOCK_STREAM, 0);
	if(sockfd < 0) {
		throw SocketError("TcpListeningSocket::listenTo(...) -> error creating the socker sockfd is " + sockfd);
    }
	int yes = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
		::close(sockfd);
		throw SocketError("setsockopt(SO_REUSEADDR) failed");
	}

	//The bind procedure is different if IPv4 or IPv6 is used
	if(sin_family == AF_INET) {

		//IPv4 case
		struct sockaddr_in myAddr;
		memset(&myAddr, 0x00, sizeof(myAddr));
		myAddr.sin_family = AF_INET;
		myAddr.sin_addr.s_addr = INADDR_ANY;
		myAddr.sin_port = htons(port);
		errorCode = bind(sockfd, (struct sockaddr*) &myAddr, sizeof(myAddr));
		if(errorCode < 0) {
			::close(sockfd);
			throw SocketError(
				std::string("TcpListeningSocket::listenTo(...) -> error bind(...) failde, errorCode is ") +
				std::to_string(errorCode) +
				std::string(", sockfd is ") +
				std::to_string(sockfd)
			);
		}

	} else {

		//IPv6 case
		struct sockaddr_in6 myAddr;
		memset(&myAddr, 0x00, sizeof(myAddr));
		myAddr.sin6_family = AF_INET6;
		myAddr.sin6_addr = in6addr_any;
		myAddr.sin6_port = htons(port);
		errorCode = bind(sockfd, (struct sockaddr*) &myAddr, sizeof(myAddr));
		if(errorCode < 0) {
			::close(sockfd);
			throw SocketError(
				std::string("TcpListeningSocket::listenTo(...) -> error bind(...) failed, errorCode is ") +
				std::to_string(errorCode) +
				std::string(", sockfd is ") +
				std::to_string(sockfd)
			);
		}

	}

	errorCode = listen(sockfd,backlog);
	if(errorCode < 0) {
        ::close(sockfd);
        throw SocketError("TcpListeningSocket::listenTo(...) -> listen(...) failed, errorCode is " + errorCode);
    }

	#endif

	listening = true;

	return listening;
}

TcpSocket TcpListeningSocket::acceptRequest() {
	SocketDescriptor newsockfd;

	//The accept procedure is "different" if IPv4 or IPv6 is used
	//The client address is stored and never used! This is just for debug reasons.
	/*if(sin_family == AF_INET) {
		//IPv4 case
		struct sockaddr_in requestingAddr;
		socklen_t requestingAddrSize = sizeof(requestingAddr);
		newsockfd = accept(sockfd, (struct sockaddr*)&requestingAddr, &requestingAddrSize);
	} else {
		//IPv6 case
		struct sockaddr_in6 requestingAddr;
		socklen_t requestingAddrSize = sizeof(requestingAddr);
		newsockfd = accept(sockfd, (struct sockaddr*)&requestingAddr, &requestingAddrSize);
	}*/
	newsockfd = accept(sockfd, NULL, NULL);

	#ifdef WINDOWS
	if(newsockfd == INVALID_SOCKET)
	#else
	if(newsockfd < 0)
	#endif
		throw SocketError("TcpListeningSocket::acceptRequest() failed");

	TcpSocket newTcpSocket = TcpSocket::fromSockfd(newsockfd, sin_family);

	return newTcpSocket;
}

void TcpListeningSocket::close() {
	if(sockfd > 0) {
		#ifdef WINDOWS
		closesocket(sockfd);
		#else
		::close(sockfd);
		#endif
		sockfd = 0;
	}
	listening = false;
}
