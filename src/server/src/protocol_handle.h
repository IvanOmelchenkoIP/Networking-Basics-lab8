// server/protcol_handle.h

#ifndef PROTOCOL_HANDLE_H
#define PROTOCOL_HANDLE_H

#include "protocol_message.h"
#include "house.h"

#include <optional>

class ProtocolServerHandleView {

public:
    void print_request_buffer(int connection_fd, string request_buffer);
    void print_response_buffer(int connection_fd, string response_buffer);
    void print_receive_error(int connection_fd);
    void print_send_error(int connection_fd);
    void print_handle_error(int connection_fd);
};

class ProtocolServerHandleModel {

private:
    static constexpr int RECEIVED_BUFFER_LENGTH = 256;

    HouseController house_controller;

public: 
    ProtocolServerHandleModel() {};
    ProtocolServerHandleModel(HouseController house_controller);
    int send_message(int connection_fd, string response_buffer);
    string receive_message(int connection_fd);
    optional<ProtocolMessage> handle_request(int connection_fd, ProtocolMessage );
};

class ProtocolServerHandleController {
    
private:
    ProtocolServerHandleModel model_handle;
    ProtocolServerHandleView view_handle;
    ProtocolMessageHandle message_handle;

public:
    ProtocolServerHandleController() {};
    ProtocolServerHandleController(ProtocolServerHandleModel model_handle, ProtocolServerHandleView view_handle, ProtocolMessageHandle message_handle);
    int handle(int connection_fd);
};

#endif