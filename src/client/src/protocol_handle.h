// client/protocol.h

#ifndef PROTOCOL_HANDLE_H
#define PROTOCOL_HANDLE_H

#include "protocol_message.h"

class ProtocolClientHandleView {

public:
    void print_send_message(string request_buffer);
    void print_receive_message(string response_buffer);
    void print_server_responses(vector<MessageResponse> responses);
    void print_send_error();
    void print_receive_error();
};

class ProtocolClientHandleModel {

private:
    static constexpr int RECEIVED_BUFFER_LENGTH = 256;

public:
    string receive_message(int client_fd);
    int send_message(int client_fd, string message);
    ProtocolMessage form_request_message(ProtocolMessageHandle message_handle, vector<MessageRequest> requests);
};

class ProtocolClientHandleController {

private:
    ProtocolClientHandleModel model_handle;
    ProtocolClientHandleView view_handle;
    ProtocolMessageHandle message_handle;

public:
    ProtocolClientHandleController() {};
    ProtocolClientHandleController(ProtocolClientHandleModel model_handle, ProtocolClientHandleView view_handle, ProtocolMessageHandle message_handle);
    int handle(int client_fd, vector<MessageRequest> requests );
};

#endif