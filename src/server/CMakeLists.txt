cmake_minimum_required(VERSION 3.8)
project(socket_server)
add_executable(${PROJECT_NAME} ./src/main.cpp ./src/server.h ./src/server.cpp ./src/house.h ./src/house.cpp ./src/protocol_message.h ./src/protocol_message.cpp ./src/protocol_handle.h ./src/protocol_handle.cpp)