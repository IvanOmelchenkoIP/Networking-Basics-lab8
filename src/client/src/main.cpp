// client/main.cpp

#include <iostream>

#include "client.h"
#include "protocol_handle.h"
#include "request.h"

using namespace std;

int main() {
    char * SERVER_NAME = "127.0.0.1";
    char * SERVER_PORT = "10000";

    optional<vector<MessageRequest>> requests = get_requests();
    if (requests == nullopt) {
        cout << "Error collecting requests to server...\nExiting program...\n";
        return 0;
    }
    vector<MessageRequest> request_vector = move(*requests);
    if (request_vector.size() == 0) {
        cout << "No requests to server...\nExiting program...\n";
        return 0;
    }

    ProtocolClientHandleModel protocol_model_handle = ProtocolClientHandleModel();
    ProtocolClientHandleView protocol_view_handle = ProtocolClientHandleView();
    ProtocolMessageHandle message_handle = ProtocolMessageHandle();
    ProtocolClientHandleController protocol_handle = ProtocolClientHandleController(protocol_model_handle, protocol_view_handle, message_handle);

    Client client = Client(SERVER_NAME, SERVER_PORT, protocol_handle);
    if (client.connect_to_server() < 0) {
        cout << "Error connecting to server...\n";
        return 0;
    }
    client.handle_messaging(request_vector);
    client.close_connection();

    return 0;
}
