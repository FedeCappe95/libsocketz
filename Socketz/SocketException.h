#ifndef __SOCKETEXCEPTION_H__
#define __SOCKETEXCEPTION_H__

#include <string>
#include "common.h"


#ifdef MSVC
#define EXPORT_CLASS_SocketException __declspec(dllexport) SocketException
#else
#define EXPORT_CLASS_SocketException SocketException
#endif


class EXPORT_CLASS_SocketException {

public:
    const static int CONNECTION_ERROR = 1;
    const static int PEER_REQUESTED_BAD_ALLOCATION = 2;
    const static int NOT_ENOUGH_MEMORY = 3;

private:
    const std::string message;
    const int errorCode;

public:
    SocketException(const std::string& message);
    SocketException(const char* message);
    SocketException(const std::string& message, const int errorCode);
    SocketException(const char* message, const int errorCode);
    std::string getMessage() const;
    int getErrorCode() const;

};

#endif
