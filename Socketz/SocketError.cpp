#include "SocketError.h"

SocketError::SocketError(const std::string& message) :
    message(message)
{}

std::string SocketError::getMessage() const {
    return message;
}
