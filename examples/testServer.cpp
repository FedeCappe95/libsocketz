#include <iostream>
#include "TcpListeningSocket.h"
#include "TcpSocket.h"

using namespace std;

void testServer() {
    TcpListeningSocket tls;
    tls.listenTo(9898); //port
    bool success;
    while(true) {
        TcpSocket ts = tls.acceptRequest(); //blocking
        cout << "New connection with " << ts.getPeerName() << endl;
        ts.sendString("Hello from server!");
        ts.close();
    }
}

int main() {
    cout << "Hello World!" << endl;
    try {
        testServer();
    } catch (SocketError e) {
        cerr << e.getMessage() << endl;
    }
    return 0;
}
