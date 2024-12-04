cmake_minimum_required(VERSION 3.8)
project(socket_client)
add_executable(${PROJECT_NAME} ./src/main.cpp ./src/client.h ./src/client.cpp ./src/protocol_message.h ./src/protocol_message.cpp ./src/protocol_handle.h ./src/protocol_handle.cpp ./src/request.h ./src/request.cpp)