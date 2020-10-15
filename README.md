# libsocketz
**Multi platform internet socket library**

The purpose of this library is to offer an interface for TCP and UDP connections 

 - as simple as possible,
 - fully accessible via C ++ constructs and
 - compatible with all major operating systems without the need for modifications.
 
However, the library is still under development, for example, support for UDP connections has not yet been implemented.

## Compile
A small sample source (*testMain.cpp*) implementing the *main* function is provided with the library code.
This source can be used as an example to understand how the library can be used and compiled with the make command (a *makefile* is already present) or via CMake (functionality in development state).
If you are using Microsoft Windows and if you want to manually compile the library do not forget to use the "WINDOWS" flag and to link also *ws2_32.lib* and *wsock32.lib*.

## Compatibility and tests
This software is and will be tested only for 64bit systems.
It "should" also run on 32bit systems, but no tests are performed.

The example was tested under:
- **Windows**
	- Windows 10 (build 2004) using *MSVC2019*
	Result: compiles but does not run
	- Windows 10 (build 2004) using *MinGW 7.3.0*
	Result: compiles but does not run
- **GNU/Linux**
	- Ubuntu 18.04 LTS using *g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0* and *make*
	Result: works
- **FreeBSD**
	- FreeBSD 12.1 using *g++ (FreeBSD Ports Collection) 9.3.0* and *make*
	Result: works
- **Mac OS X**
	- Mac OS X Catalina
	Result: not tested yet

## License
See the *LICENSE* file.
