#ifndef __SOCKETERROR_H__
#define __SOCKETERROR_H__

#include <string>

class SocketError {

private:
    const std::string message;

public:
    SocketError(const std::string& message);
    SocketError(const char* message);
    std::string getMessage() const;

};

#endif
