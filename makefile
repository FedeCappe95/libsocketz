#Costants
INC=-I./Socketz/
#OPT=-Wall -Wextra -Wno-reourder -g
OPT=-g
COMMONDEP=./Socketz/common.h


#Make entrypoint
start:
	@echo   "===================================="
	@echo   "= Compiling                        ="
	@printf "= %-32s =\n" "`date`"
	@echo   "====================================\n"
	@mkdir -p bin
	@make ./bin/libsocketz.so

#All (it just makes everything)
all: ./bin/libsocketz.so ./bin/testClient ./bin/testServer

#Test client
./bin/testClient: ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketException.o ./bin/common.o ./examples/testClient.cpp $(COMMONDEP)
	@echo "\n=== Client executable ==="
	@echo "  >> $@"
	@g++ ./examples/testClient.cpp ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketException.o ./bin/common.o	$(OPT) $(INC) -o $@
	@echo ""

#Test server
./bin/testServer: ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketException.o ./bin/common.o ./examples/testServer.cpp $(COMMONDEP)
	@echo "\n=== Server executable ==="
	@echo "  >> $@"
	@g++ ./examples/testServer.cpp ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketException.o ./bin/common.o	$(OPT) $(INC) -o $@
	@echo ""

#Object files of libsocketz
./bin/TcpSocket.o: ./Socketz/TcpSocket.cpp ./Socketz/TcpSocket.h $(COMMONDEP)
	@echo "  >> $@"
	@g++ -fPIC -c ./Socketz/TcpSocket.cpp $(OPT) $(INC)          -o $@

./bin/TcpListeningSocket.o: ./Socketz/TcpListeningSocket.cpp ./Socketz/TcpListeningSocket.h $(COMMONDEP)
	@echo "  >> $@"
	@g++ -fPIC -c ./Socketz/TcpListeningSocket.cpp $(OPT) $(INC) -o $@

./bin/SocketException.o: ./Socketz/SocketException.cpp ./Socketz/SocketException.h $(COMMONDEP)
	@echo "  >> $@"
	@g++ -fPIC -c ./Socketz/SocketException.cpp $(OPT) $(INC)        -o $@

./bin/common.o: ./Socketz/common.cpp $(COMMONDEP)
	@echo "  >> $@"
	@g++ -fPIC -c ./Socketz/common.cpp $(OPT) $(INC)             -o $@

./bin/libsocketz.so: ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketException.o ./bin/common.o
	@echo "  >> $@"
	@g++ ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketException.o ./bin/common.o -shared -o ./bin/libsocketz.so

#Clean
clean:
	rm -f ./bin/*
