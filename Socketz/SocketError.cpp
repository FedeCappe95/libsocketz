#include "SocketError.h"

SocketError::SocketError(const std::string& message) :
    message(message)
{}

SocketError::SocketError(const char* message) :
    message(std::string(message))
{}

std::string SocketError::getMessage() const {
    return message;
}
