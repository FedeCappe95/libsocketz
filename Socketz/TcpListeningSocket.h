#ifndef __TCPLISTENINGSOCKET_H__
#define __TCPLISTENINGSOCKET_H__


#ifdef WINDOWS
#include <Ws2tcpip.h>
#include <Windows.h>
#else
#include <netinet/in.h>
#endif

#include "TcpSocket.h"
#include "common.h"

class TcpListeningSocket {
private:
	SocketDescriptor sockfd;
	bool listening;
	short sin_family; //IPv4 (AF_INET) or IPv6 (AF_INET6)
	static const uint16_t DEFAULT_BACKLOG = 100U;

public:
	TcpListeningSocket(const InternetProtocol internetProtocol = IPv4);
	~TcpListeningSocket();
	bool listenTo(const uint16_t port, const uint16_t backlog = DEFAULT_BACKLOG);
	TcpSocket acceptRequest();
	void close();
};


#endif
