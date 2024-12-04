// client/client.cpp

#include "client.h"

#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

Client::Client(char* server_name, char* server_port, ProtocolClientHandleController protocol_handle) {
    this->server_name = server_name;
    this->server_port = server_port;
    this->protocol_handle = protocol_handle;
};

Client::~Client() {
    if (client_fd) close(client_fd);
}

int Client::connect_to_server() {
    addrinfo* host_info = get_server_info();
    if (host_info == nullptr) return 1;
    client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_fd < 0) return 1;
    sockaddr_in * socket_addr = (sockaddr_in *)host_info->ai_addr;
    socklen_t socket_len = host_info->ai_addrlen;
    if (connect(client_fd, (sockaddr *)socket_addr, socket_len) < 0) return -1;
    print_connection_message();
    return 0;
}

addrinfo* Client::get_server_info() {
    struct addrinfo * server_info;
    addrinfo server_info_hints = addrinfo();
    server_info_hints.ai_family = AF_INET;
    server_info_hints.ai_socktype = SOCK_STREAM;
    server_info_hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(server_name, server_port, &server_info_hints, &server_info) != 0) return nullptr;
    return server_info;
}

void Client::handle_messaging(vector<MessageRequest> requests) {
    print_messaging_start();
    if (protocol_handle.handle(client_fd, requests) < 0) print_messaging_error();
    else print_messaging_success();
}

int Client::close_connection() {
    print_closed_message();
    close(client_fd);
}

void Client::print_connection_message() {
    cout << "Connected to server on IP address " << server_name 
            << " port " << server_port << "\nClient fd " << client_fd << "\n";
}

void Client::print_messaging_start() {
    cout << "Starting messaging process with server...\n";
}

void Client::print_messaging_success() {
    cout << "Successfully sent a request to server and received a response\n";
}
 
void Client::print_messaging_error() {
    cout << "Error encountered while messaging with server\n";
}

void Client::print_closed_message() {
    cout << "Closing connection with server...\n";
}
