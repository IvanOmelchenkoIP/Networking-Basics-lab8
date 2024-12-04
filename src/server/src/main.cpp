// server/main.cpp

#include <iostream>

#include "server.h"

using namespace std;

int main() {
    char * HOSTNAME = "127.0.0.1";
    char * PORT = "10000";

    HouseModel house_model = HouseModel();
    HouseView house_view = HouseView();

    HouseController house_controller = HouseController(house_model, house_view);
    ProtocolServerHandleModel protocol_handle_model = ProtocolServerHandleModel(house_controller);
    ProtocolServerHandleView protocol_handle_view = ProtocolServerHandleView();
    ProtocolMessageHandle message_handle = ProtocolMessageHandle();
    ProtocolServerHandleController protocol_handle = ProtocolServerHandleController(protocol_handle_model, protocol_handle_view, message_handle);

    Server server = Server(HOSTNAME, PORT, protocol_handle);
    server.start();

    return 0;
}
