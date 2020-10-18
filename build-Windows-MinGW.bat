@echo off
rem Configuration
set COMPILER_PATH="E:\Qt\Tools\mingw730_64\bin\"
set COMPILER_EXEC=%COMPILER_PATH%\x86_64-w64-mingw32-g++.exe
set COMPILER_FLAG=-DWINDOWS -I..\Socketz -DMINGW -L./

mkdir bin

cd bin
del /Q .\*

echo ===== Building the library =====
%COMPILER_EXEC% %COMPILER_FLAG% ..\Socketz\common.cpp ..\Socketz\SocketError.cpp ..\Socketz\TcpListeningSocket.cpp ..\Socketz\TcpSocket.cpp -lws2_32 -lntdll -shared -o libsocketz.dll

echo ===== Example: testClient =====
%COMPILER_EXEC% %COMPILER_FLAG% ..\examples\testClient.cpp -lws2_32 -lntdll -lsocketz -o testClient.exe

echo ===== Example: testServer =====
%COMPILER_EXEC% %COMPILER_FLAG% ..\examples\testServer.cpp -lws2_32 -lntdll -lsocketz -o testServer.exe

echo ===== Copying required libraries =====
copy %COMPILER_PATH%\"libgcc_s_seh-1.dll" .\
copy %COMPILER_PATH%\"libstdc++-6.dll" .\

cd ..

pause
