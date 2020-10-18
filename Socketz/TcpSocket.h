#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#ifdef WINDOWS
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#endif

#include <string>
#include <vector>
#include "common.h"


#ifdef MSVC
#define EXPORT_CLASS_TcpSocket __declspec(dllexport) TcpSocket
#else
#define EXPORT_CLASS_TcpSocket TcpSocket
#endif


class EXPORT_CLASS_TcpSocket {

private:
	int connectionStatus; //0 connected, -1 error, -2 not connected
	short sin_family; //IPv4 (AF_INET) or IPv6 (AF_INET6)
	SocketDescriptor sockfd;

private:
	inline void sendOrDie(const void* buffer, const uint32_t size);
	inline void receiveOrDie(void* buffer, const uint32_t howManyBytes);

public:
	TcpSocket(const InternetProtocol internetProtocol = IPv4);
	~TcpSocket();

	/**
	 * Connect to the host specified by "ip". The host has to listen on port "port"
	*/
	bool connectTo(const std::string& ip, const uint16_t port);

	int send(const void* buffer, const uint32_t size);
	int send(const std::vector<byte>& buffer);

	void sendObject(const std::vector<byte>& buffer);
	void sendObject(const void* buffer, const uint32_t size);
	void sendString(const std::string& str);

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
	SocketDescriptor getSockfd();

public:
	/**
	 * Utility function: convert a socket descriptor to a TcpSocket instance
	*/
	static TcpSocket fromSockfd(SocketDescriptor sockfd, const short sin_family);

};


#endif
