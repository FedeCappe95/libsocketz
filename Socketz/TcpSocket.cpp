#include "TcpSocket.h"
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

using namespace std;
using namespace SocketzInternals;


TcpSocket::TcpSocket(const InternetProtocol internetProtocol) :
	sockfd(0),
	connectionStatus(-2)
{
	sin_family = (internetProtocol == IPv4 ? AF_INET : AF_INET6);
	sockfd = socket(sin_family, SOCK_STREAM, 0);
	if(sockfd < 0) {
		throw SocketError("Error during TcpSocket constructor. sockfd=" + sockfd);
    }
}

TcpSocket::~TcpSocket() {
	if(sockfd > 0) {
		closesocket(sockfd);
	}
}

bool TcpSocket::connectTo(const std::string& ip, const uint16_t port) {
	if(isConnected()){
		throw SocketError("Already connected socket");
	}

	connectionStatus = -1;

	if(sin_family == AF_INET) {
		//Connect using IPv4
		struct sockaddr_in serverAddr;
		memset(&serverAddr, 0x00, sizeof(serverAddr));
		serverAddr.sin_family = sin_family;
	    serverAddr.sin_port = htons(port);
		serverAddr.sin_addr = ipv4AddressFromString(ip);
		connectionStatus = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	} else if(sin_family == AF_INET6) {
		//Connect using IPv6
		struct sockaddr_in6 serverAddr;
		memset(&serverAddr, 0x00, sizeof(serverAddr));
		serverAddr.sin6_family = sin_family;
	    serverAddr.sin6_port = htons(port);
		serverAddr.sin6_addr = ipv6AddressFromString(ip);
		connectionStatus = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	}

	return isConnected();
}

int TcpSocket::send(const void* buffer, const uint32_t size) {
	if(size > INT_MAX){
		throw SocketError("TcpSocket::send(...) -> size too big");
	}
	if(!isConnected()){
		throw SocketError("It is impossible to send something using a non connected socket");
	}

	//old version: int actuallySend = write(sockfd, buffer, size);
	#ifdef WINDOWS
	int actuallySent = ::send(sockfd, (const char*)buffer, size, 0);
	#else
	int actuallySent = ::send(sockfd, buffer, size, 0);
	#endif

	return actuallySent;
}

int TcpSocket::send(const std::vector<byte>& buffer) {
	return send((const byte*)buffer.data(), buffer.size());
}

bool TcpSocket::sendObject(const std::vector<byte>& buffer) {
	uint32_t size = buffer.size();
	if(send((byte*)&size, sizeof(size)) != sizeof(size))
		return false;
	if(send((const byte*)buffer.data(), buffer.size()) != (int)buffer.size())
		return false;
	return true;
}

bool TcpSocket::sendObject(const byte* buffer, const uint32_t size) {
	if(send((byte*)&size, sizeof(size)) != sizeof(size))
		return false;
	if(send(buffer, size) != (int)size)
		return false;
	return true;
}

bool TcpSocket::sendString(const std::string& str) {
	uint32_t size = str.length();
	if(send((byte*)&size, sizeof(size)) != sizeof(size))
		return false;
	if(send(str.c_str(), size) != (int)size)
		return false;
	return true;
}

int TcpSocket::receive(void* buffer, const uint32_t howManyBytes) {
	if(howManyBytes > INT_MAX){
		throw SocketError("TcpSocket::receive(...) -> howManyBytes too big");
	}
	if(!isConnected()){
		throw SocketError("It is impossible to send something using a non connected socket");
	}

	#ifdef WINDOWS
	int actuallyReceived = recv(sockfd, (char*)buffer, howManyBytes, MWMO_WAITALL);
	#else
	int actuallyReceived = recv(sockfd, buffer, howManyBytes, MSG_WAITALL);
	#endif

	return actuallyReceived;
}

std::vector<byte> TcpSocket::receive(const uint32_t howManyBytes) {
	if(howManyBytes > INT_MAX){
		throw SocketError("TcpSocket::receive(...) -> howManyBytes too big");
	}
	if(!isConnected()){
		throw SocketError("Impossible to send something using a non connected socket");
	}

	std::vector<byte> receiveVector(howManyBytes);
	int actuallyReceived = receive(receiveVector.data(), howManyBytes);
	if(actuallyReceived < 0){
		cerr << "Error: TcpSocket::receive(...) can not receive correctly";
		return vector<byte>();
	}
	if((uint32_t)actuallyReceived != howManyBytes)
		receiveVector.resize(actuallyReceived);

	return receiveVector;
}

std::vector<byte> TcpSocket::receiveObject() {
	uint32_t size;
	if(receive((byte*)&size, sizeof(size)) != sizeof(size))
		return std::vector<byte>(0);
	std::vector<byte> buffer(size);
	if(receive(buffer.data(), size) != (int)size)
		return std::vector<byte>(0);
	return buffer;
}

std::string TcpSocket::receiveString() {
	uint32_t size;
	if(receive((byte*)&size, sizeof(size)) != sizeof(size))
		return "";
	std::vector<byte> buffer(size+1);
	if(receive(buffer.data(), size) != (int)size)
		return "";
	buffer[size] = 0;
	return string((char*)buffer.data());
}

void TcpSocket::close() {
	if(sockfd > 0){
		closesocket(sockfd);
		sockfd = 0;
	}
	connectionStatus = -2;
}

TcpSocket TcpSocket::fromSockfd(int sockfd, const short sin_family) {
	if(sockfd < 0){
		throw SocketError("TcpSocket::fromSockfd(...) -> ivalid sockfd");
	}

	TcpSocket newTcpSocket;
	newTcpSocket.sockfd = sockfd;
	newTcpSocket.connectionStatus = 0;
	newTcpSocket.sin_family = sin_family;
	return newTcpSocket;
}

bool TcpSocket::isConnected() {
	return (connectionStatus == 0);
}

std::string TcpSocket::getPeerName() const {
    if(sin_family == AF_INET) {

        //IPv4 case
        struct sockaddr_in addr;
        socklen_t addr_size = sizeof(addr);
        if(getpeername(sockfd, (struct sockaddr *)&addr, &addr_size) < 0) {
            throw SocketError("getPeerName() -> error in getpeername(...)");
        }
        return ipv4AddressToString(&addr.sin_addr);

    } else {

        //IPv6 case
        struct sockaddr_in6 addr;
        socklen_t addr_size = sizeof(addr);
        if(getpeername(sockfd, (struct sockaddr*)&addr, &addr_size) < 0) {
            throw SocketError("getPeerName() -> error in getpeername(...)");
        }
        return ipv6AddressToString(&addr.sin6_addr);
    }
}

int TcpSocket::getSockOptions() {
	int errorCode;
	#ifdef WINDOWS
	int errorCodeSize = sizeof(errorCode);
	getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&errorCode, &errorCodeSize);
	#else
	socklen_t errorCodeSize = sizeof(errorCode);
	getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &errorCode, &errorCodeSize);
	#endif
	return errorCode;
}

int TcpSocket::getSockfd() {
	return sockfd;
}
