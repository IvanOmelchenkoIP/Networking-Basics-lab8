// client/client.h

#ifndef CLIENT_H
#define CLIENT_H

#include "protocol_handle.h"

#include <netdb.h>

class Client {

private:
    char * server_name;
    char * server_port;
    int client_fd;
    ProtocolClientHandleController protocol_handle;

    addrinfo* get_server_info();

    void print_connection_message();
    void print_messaging_start();
    void print_messaging_success();
    void print_messaging_error();
    void print_closed_message();

public:
    Client(char* server_name, char* server_port, ProtocolClientHandleController protocol_handle);
    ~Client();
    int connect_to_server();
    void handle_messaging(vector<MessageRequest> requests);
    int close_connection();
};

#endif