#include "TcpListeningSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <climits>
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

#ifdef WINDOWS
#else
#define closesocket ::close
#endif

TcpListeningSocket::TcpListeningSocket(const InternetProtocol internetProtocol) :
	sockfd(0),
	listening(false)
{
	sin_family = (internetProtocol == IPv4 ? AF_INET : AF_INET6);
	sockfd = socket(sin_family, SOCK_STREAM, 0);
	if(sockfd < 0) {
		throw SocketError("Error in TcpListeningSocket constructor. sockfd=" + sockfd);
    }

	//set socket option (setsockopt) to set SO_REUSEADDR true.
	//It allows the socket to be bound <<to an address that is already in use>>.
	//The procedure is slightly different from Windows to Linux.
	#ifdef WINDOWS
	const BOOL yes = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) < 0)
		throw SocketError("setsockopt(SO_REUSEADDR) failed");
	#else
	int yes = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw SocketError("setsockopt(SO_REUSEADDR) failed");
	#endif
}

TcpListeningSocket::~TcpListeningSocket() {
	if(sockfd > 0) {
		closesocket(sockfd);
	}
}


bool TcpListeningSocket::listenTo(const uint16_t port, const uint16_t backlog){
	if(listening)
		throw SocketError("TcpListeningSocket::listenTo(...) -> socket already in listen state");

	//The bind procedure is different if IPv4 or IPv6 is used
	if(sin_family == AF_INET) {

		//IPv4 case
		struct sockaddr_in myAddr;
		memset(&myAddr, 0x00, sizeof(myAddr));
		myAddr.sin_family = AF_INET;
		myAddr.sin_addr.s_addr = INADDR_ANY;
		myAddr.sin_port = htons(port);
		if(bind(sockfd, (struct sockaddr*) &myAddr, sizeof(myAddr)) < 0) {
			throw SocketError("TcpListeningSocket::listenTo(...) -> errore during bind()");
		}

	} else {

		//IPv6 case
		struct sockaddr_in6 myAddr;
		memset(&myAddr, 0x00, sizeof(myAddr));
		myAddr.sin6_family = AF_INET6;
		myAddr.sin6_addr = in6addr_any;
		myAddr.sin6_port = htons(port);
		if(bind(sockfd, (struct sockaddr*) &myAddr, sizeof(myAddr)) < 0) {
			throw SocketError("TcpListeningSocket::listenTo(...) -> errore during bind()");
		}

	}

	listen(sockfd,backlog);

	listening = true;

	return listening;
}

TcpSocket TcpListeningSocket::acceptRequest() {
	int newsockfd;

	//The accept procedure is different if IPv4 or IPv6 is used
	if(sin_family == AF_INET) {

		//IPv4 case
		struct sockaddr_in requestingAddr;
		socklen_t requestingAddrSize = sizeof(requestingAddr);
		newsockfd = accept(sockfd, (struct sockaddr*)&requestingAddr, &requestingAddrSize);

	} else {

		//IPv6 case
		struct sockaddr_in6 requestingAddr;
		socklen_t requestingAddrSize = sizeof(requestingAddr);
		newsockfd = accept(sockfd, (struct sockaddr*)&requestingAddr, &requestingAddrSize);

	}

	if(newsockfd < 0)
		throw SocketError("TcpListeningSocket::acceptRequest() -> error");

	TcpSocket newTcpSocket = TcpSocket::fromSockfd(newsockfd, sin_family);

	return newTcpSocket;
}

void TcpListeningSocket::close() {
	if(sockfd > 0){
		closesocket(sockfd);
		sockfd = 0;
	}
	listening = false;
}
