// server/server.h

#ifndef SERVER_H
#define SERVER_H

#include "protocol_handle.h"

#include <netdb.h>

class Server {

private:
    char * hostname;
    char * port;
    int server_fd;
    ProtocolServerHandleController protocol_handle;

    addrinfo* get_host_info();
    int bind_socket(addrinfo* host_info);
    void accept_clients();
    void handle_client(int connection_fd);

    void print_start_message();
    void print_client_connected(int connection_fd);
    void print_client_closed(int connection_fd);
    void print_handle_error(int connection_fd);
    void print_handle_success(int connection_fd);

public:

    Server(char* hostname, char* port, ProtocolServerHandleController protocol_handle);
    ~Server();

    int start();

};

#endif
