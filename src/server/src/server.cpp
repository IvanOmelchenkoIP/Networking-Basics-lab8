// server/server.cpp

#include "server.h"

#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

Server::Server(char* hostname, char* port, ProtocolServerHandleController protocol_handle) {
    this->hostname = hostname;
    this->port = port;
    this->protocol_handle = protocol_handle;
}

Server::~Server() {
    close(server_fd);
}

int Server::start() {
    addrinfo* host_info = get_host_info();
    if (host_info == nullptr) return 1;
    if (bind_socket(host_info) == 1) return 1;
    freeaddrinfo(host_info);
    if (listen(server_fd, 1) < 0) return 1;

    print_start_message();

    accept_clients();
    return 0;
}

addrinfo* Server::get_host_info() {
    struct addrinfo * host_info;
    addrinfo host_info_hints = addrinfo();
    host_info_hints.ai_family = AF_INET;
    host_info_hints.ai_socktype = SOCK_STREAM;
    host_info_hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(hostname, port, &host_info_hints, &host_info) != 0) return nullptr;
    return host_info;
}

int Server::bind_socket(addrinfo* host_info) {
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0) return 1;
    sockaddr_in * socket_addr = (sockaddr_in *)host_info->ai_addr;
    socklen_t socket_len = host_info->ai_addrlen;
    if (bind(server_fd, (sockaddr *)socket_addr, socket_len) < 0) return 1;
    return 0;
}

void Server::accept_clients() {
    while (true) {
        sockaddr cli_addr = sockaddr();
        socklen_t cli_len = sizeof(cli_addr);
        int connection_fd = accept(server_fd, &cli_addr, &cli_len);
        if (connection_fd < 0) continue;
        handle_client(connection_fd);
        close(connection_fd);
    }
}

void Server::handle_client(int connection_fd) {
    if (protocol_handle.handle(connection_fd) < 0) print_handle_error(connection_fd);
    else print_handle_success(connection_fd);
}

void Server::print_start_message() {
    cout << "Started server at IP address " << hostname 
            << " port " << port << "\nServer fd " << server_fd << "\n";
}

void Server::print_client_connected(int connection_fd) {
    cout << "Client connected. Processing messaging. Connection fd: " << connection_fd << "\n";
}

void Server::print_client_closed(int connection_fd) {
    cout << "Closing connection with client. Connection fd: " << connection_fd << "\n";
}

void Server::print_handle_error(int connection_fd) {
    cout << "Error processing client request. Closing connection at fd " << connection_fd << "\n";
}

void Server::print_handle_success(int connection_fd) {
    cout << "Client was handled successfully at fd " << connection_fd << ". Closing connection\n";
}
