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
#include <climits>


#ifdef MSVC
#define EXPORT_CLASS_TcpSocket __declspec(dllexport) TcpSocket
#else
#define EXPORT_CLASS_TcpSocket TcpSocket
#endif


#define DEFAULT_SOCKET_MAX_OBJECT_SIZE (uint32_t)INT_MAX
#define ABSOLUTE_MAX_SOCKET_MAX_OBJECT_SIZE (uint32_t)INT_MAX
#define ABSOLUTE_MIN_SOCKET_MAX_OBJECT_SIZE 1U


//#define getSockOptions(L, O, V) getSockOptions(L, O, V, sizeof(*(V)))


enum SocketStatus {CONNECTED, CLOSED, S_ERROR};


class EXPORT_CLASS_TcpSocket {

private:
	SocketStatus socketStatus;
	short sin_family; //IPv4 (AF_INET) or IPv6 (AF_INET6)
	SocketDescriptor sockfd;
	uint32_t maxObjectSize;

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
	inline bool isConnected() const;
	inline SocketStatus getSocketStatus() const;

	std::string getPeerName() const;

	uint32_t getMaxObjectSize() const;
	bool setMaxObjectSize(const uint32_t maxObjectSize);

	/**
	 * Old-style socket options
	*/
	inline bool getSockOptions(const int level, const int optname, void* outValue, const uint32_t outValueSize) const;
	inline bool getSockOptions(const int level, const int optname, bool* outValue) const;
	inline bool getSockOptions(const int level, const int optname, char* outValue) const;
	inline bool getSockOptions(const int level, const int optname, short* outValue) const;
	inline bool getSockOptions(const int level, const int optname, int* outValue) const;
	inline bool setSockOptions(const int level, const int optname, const void* inValue, const uint32_t inValueSize);
	inline bool setSockOptions(const int level, const int optname, const bool inValue);
	inline bool setSockOptions(const int level, const int optname, const char inValue);
	inline bool setSockOptions(const int level, const int optname, const short inValue);
	inline bool setSockOptions(const int level, const int optname, const int inValue);

	/**
	 * Return the old-style socket descriptor.
	 * This function is only for debugging purpose and should not be used.
	*/
	SocketDescriptor getSockfd() const;

public:
	/**
	 * Utility function: convert a socket descriptor to a TcpSocket instance
	*/
	static TcpSocket fromSockfd(SocketDescriptor sockfd, const short sin_family);
	static int getLastSocketErrorCode();
	static std::string getLastSockerErrorString();

};


#endif
