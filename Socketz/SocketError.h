#ifndef __SOCKETERROR_H__
#define __SOCKETERROR_H__

#include <string>
#include "common.h"


#ifdef MSVC
#define EXPORT_CLASS_SocketError __declspec(dllexport) SocketError
#else
#define EXPORT_CLASS_SocketError SocketError
#endif


class EXPORT_CLASS_SocketError {

private:
    const std::string message;

public:
    SocketError(const std::string& message);
    SocketError(const char* message);
    std::string getMessage() const;

};

#endif
