#include <iostream>
#include "TcpListeningSocket.h"
#include "TcpSocket.h"

using namespace std;

void testServer() {
    TcpListeningSocket tls;
    tls.listenTo(9898); //port
    while(true) {
        TcpSocket ts = tls.acceptRequest(); //blocking
        ts.sendString("Hello from server!");
        ts.close();
    }
}

int main() {
    cout << "Hello World!" << endl;
    testServer();
    return 0;
}
