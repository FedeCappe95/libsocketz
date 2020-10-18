# libsocketz
**Multi platform network socket library**

The purpose of this library is to offer an interface for TCP and UDP connections

 - as simple as possible,
 - fully accessible via C ++ constructs and
 - compatible with all major operating systems without the need for modifications.

However, the library is still under development (alpha stage), for example, support for UDP connections has not yet been implemented.

## Build on Linux / FreeBSD / Cygwin / Mac OS X
There is a makefile! :D
Just run *make* or *make start* to build the library. Use *make all* to build everything (example included).
For a list of tested compilers and systems, see the appropriate paragraph (*Compatibility and tests*).
Release version coming soon. :)

## Build on Windows
You have three options:
- Install **Cygwin** and follow the instructions in the appropriate paragraph.
- Use **MinGW**. You need to customize the script *build-Windows-MinGW.bat* by adding the path to your MinGW installation. Once this is done, just run the script.
- Use **MSVC2019**. You need to customize the script by adding paths relative to the MSCV2019 installation. Once this is done, just run the script.

For a list of tested compilers and systems, see the appropriate paragraph (*Compatibility and tests*).
Release version coming soon. :)

## Compatibility and tests
This software is and will be tested only on 64bit systems.
It "should" also run on 32bit systems, but no tests are performed.

Some tests were (successfully) run on:
- **Windows**
	- Windows 10 (build 2004) using *MSVC2019*
	- Windows 10 (build 2004) using *MinGW 7.3.0*
	- Cygwin on Windows 10 (build 2004) using *gcc-g++ 10.2.0-1*
- **GNU/Linux**
	- Ubuntu 18.04 LTS using *g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0*
- **FreeBSD**
	- FreeBSD 12.1 using *g++ (FreeBSD Ports Collection) 9.3.0*
- **Mac OS X**
    - Mac OS X Catalina 10.15 using *Xcode 11.3.1*

## License
See the *LICENSE* file. Basically everything is licensed under LGPL v2.1.
