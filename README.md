# libsocketz
**Multi platform internet socket library**

The purpose of this library is to offer an interface for TCP and UDP connections 

 - as simple as possible,
 - fully accessible via C ++ constructs and
 - compatible with all major operating systems without the need for modifications.
 
However, the library is still under development, for example, support for UDP connections has not yet been implemented.

## Build on Linux/FreeBSD/Cygwin
There is a makefile! :D
Just run *make* or *make start* to build the library. Use *make all* to build everything (example included).
For a list a compatible systems see "Compatibility and tests".
Release version coming soon. :)

## Build on Windows
There are no automatic building systems for now. You can wait for release versions or compile manually.
For example, using *MSVC2019*, you can run:
    <pre><code>cd bin
    set COMPILER_PATH=cl.exe
    set COMPILER_FLAG=/DWINDOWS /I..\Socketz /EHs
    %COMPILER_PATH% %COMPILER_FLAG% ..\Socketz\common.cpp ..\Socketz\SocketError.cpp ..\Socketz\TcpListeningSocket.cpp ..\Socketz\TcpSocket.cpp ..\examples\testClient.cpp /FetestClient.exe
    %COMPILER_PATH% %COMPILER_FLAG% ..\Socketz\common.cpp ..\Socketz\SocketError.cpp ..\Socketz\TcpListeningSocket.cpp ..\Socketz\TcpSocket.cpp ..\examples\testServer.cpp /FetestServer.exe
    cd ..</code></pre>

## Build on Mac OS X
WIP

## Compatibility and tests
This software is and will be tested only for 64bit systems.
It "should" also run on 32bit systems, but no tests are performed.

Some tests were run on:
- **Windows**
	- Windows 10 (build 2004) using *MSVC2019* → it works
	- Windows 10 (build 2004) using *MinGW 7.3.0* → I almost did it
	- Cygwin on Windows 10 (build 2004) using *gcc-g++ 10.2.0-1* → it works
- **GNU/Linux**
	- Ubuntu 18.04 LTS using *g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0* and *make* → it works
- **FreeBSD**
	- FreeBSD 12.1 using *g++ (FreeBSD Ports Collection) 9.3.0* and *make* → it works
- **Mac OS X**
	- WIP

## License
See the *LICENSE* file. Basically everything is licensed under LGPL v2.1.
