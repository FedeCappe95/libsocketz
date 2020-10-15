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
	@make all

#All (it just makes the test main)
all: ./bin/testMain

#Test main
./bin/testMain: ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketError.o ./bin/common.o $(COMMONDEP)

	@echo "\n=== Server executable ==="
	@echo "  >> $@"
	@g++ ./testMain.cpp ./bin/TcpSocket.o ./bin/TcpListeningSocket.o ./bin/SocketError.o ./bin/common.o	$(OPT) $(INC) -o $@
	@echo ""

#Object files
./bin/TcpSocket.o: ./Socketz/TcpSocket.cpp ./Socketz/TcpSocket.h $(COMMONDEP)
	@echo "  >> $@"
	@g++ -c ./Socketz/TcpSocket.cpp $(OPT) $(INC)          -o $@

./bin/TcpListeningSocket.o: ./Socketz/TcpListeningSocket.cpp ./Socketz/TcpListeningSocket.h $(COMMONDEP)
	@echo "  >> $@"
	@g++ -c ./Socketz/TcpListeningSocket.cpp $(OPT) $(INC) -o $@

./bin/SocketError.o: ./Socketz/SocketError.cpp ./Socketz/SocketError.h $(COMMONDEP)
	@echo "  >> $@"
	@g++ -c ./Socketz/SocketError.cpp $(OPT) $(INC)        -o $@

./bin/common.o: ./Socketz/common.cpp $(COMMONDEP)
	@echo "  >> $@"
	@g++ -c ./Socketz/common.cpp $(OPT) $(INC)             -o $@

#Clean
clean:
	rm -f ./bin/*.o
	rm -f ./bin/testMain
