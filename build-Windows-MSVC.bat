@echo off
rem Configuration
set COMPILER_PATH="E:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.27.29110\bin\Hostx64\x64"
set VCVARS_PATH="E:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build"
set COMPILER_EXEC=%COMPILER_PATH%\cl.exe
set COMPILER_FLAG=/DWINDOWS /DMSVC /I..\Socketz /EHa

mkdir bin

cd bin
del /Q .\*

call %VCVARS_PATH%\vcvars64.bat

echo ===== Building the library =====
%COMPILER_EXEC% %COMPILER_FLAG% /c ..\Socketz\common.cpp ..\Socketz\SocketException.cpp ..\Socketz\TcpListeningSocket.cpp ..\Socketz\TcpSocket.cpp /Fo
%COMPILER_PATH%\link.exe /DLL /OUT:libsocketz.dll .\*.obj
del *.obj

echo ===== Example: testClient =====
%COMPILER_EXEC% %COMPILER_FLAG% ..\examples\testClient.cpp /FetestClient.exe /link libsocketz.lib

echo ===== Example: testServer =====
%COMPILER_EXEC% %COMPILER_FLAG% ..\examples\testServer.cpp /FetestServer.exe /link libsocketz.lib

cd ..

pause
