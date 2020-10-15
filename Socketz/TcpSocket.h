#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#ifdef WINDOWS
#include <Ws2tcpip.h>
#include <Windows.h>
#else
#include <netinet/in.h>
#endif

#include <string>
#include <vector>
#include "common.h"

class TcpSocket {

private:
	int connectionStatus; //0 connected, -1 error, -2 not connected
	short sin_family; //IPv4 (AF_INET) or IPv6 (AF_INET6)
	SocketDescriptor sockfd;

public:
	TcpSocket(const InternetProtocol internetProtocol = IPv4);
	~TcpSocket();

	/**
	 * Connect to the host specified by "ip". The host has to listen on port "port"
	*/
	bool connectTo(const std::string& ip, const uint16_t port);

	int send(const void* buffer, const uint32_t size);
	int send(const std::vector<byte>& buffer);

	bool sendObject(const std::vector<byte>& buffer);
	bool sendObject(const byte* buffer, const uint32_t size);
	bool sendString(const std::string& str);

	int receive(void* buffer, const uint32_t howManyBytes);
	std::vector<byte> receive(const uint32_t howManyBytes);

	std::vector<byte> receiveObject();
	std::string receiveString();

	void close();
	inline bool isConnected();

	std::string getPeerName() const;

	/**
	 * Old-style socket option
	*/
	int getSockOptions();

	/**
	 * Return the old-style socket descriptor.
	 * This function is only for debugging purpose and should not be used.
	*/
	int getSockfd();

public:
	/**
	 * Utility function: convert a socket descriptor to a TcpSocket instance
	*/
	static TcpSocket fromSockfd(int sockfd, const short sin_family);

};


#endif
