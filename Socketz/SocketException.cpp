#include "SocketException.h"

SocketException::SocketException(const std::string& message) :
    message(message),
    errorCode(CONNECTION_ERROR)
{}

SocketException::SocketException(const char* message) :
    message(std::string(message)),
    errorCode(CONNECTION_ERROR)
{}

SocketException::SocketException(const std::string& message, const int errorCode) :
    message(message),
    errorCode(errorCode)
{}

SocketException::SocketException(const char* message, const int errorCode) :
    message(std::string(message)),
    errorCode(errorCode)
{}

std::string SocketException::getMessage() const {
    return message;
}

int SocketException::getErrorCode() const {
    return errorCode;
}
