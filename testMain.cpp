#include <iostream>
#include <string>
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

void testClient() {
    TcpSocket ts;
    bool success = ts.connectTo("127.0.0.1",9898);
    if(!success) {
        cerr << "Connection error" << endl;
        ts.close();
        return;
    }
    string messageFromServer = ts.receiveString();
    ts.close();
    cout << "The server send: " << messageFromServer << endl;
}

int main() {
    cout << "Hello World!" << endl;
    return 0;
}
