#include <iostream>
#include "TcpListeningSocket.h"
#include "TcpSocket.h"

using namespace std;

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
    cout << "The server sent: " << messageFromServer << endl;
}

int main() {
    cout << "Hello World!" << endl;
    testClient();
    return 0;
}
