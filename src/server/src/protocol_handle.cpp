// server/protocol_handle.cpp

#include "protocol_handle.h"

#include <netdb.h>
#include <iostream>
#include <string.h>

using namespace std;

ProtocolServerHandleController::ProtocolServerHandleController(ProtocolServerHandleModel model_handle, ProtocolServerHandleView handle_view, ProtocolMessageHandle message_handle) {
    this->model_handle = model_handle;
    this->view_handle = handle_view;
    this->message_handle = message_handle;
}

int ProtocolServerHandleController::handle(int connection_fd) {
    string request_buffer = model_handle.receive_message(connection_fd);
    if (request_buffer.length() == 0) {
        view_handle.print_receive_error(connection_fd);
        return -1;
    }
    view_handle.print_request_buffer(connection_fd, request_buffer);
    ProtocolMessage request_message = message_handle.parse_buffer(request_buffer);
    optional<ProtocolMessage> response = model_handle.handle_request(connection_fd, request_message);
    if (response == nullopt) {
        view_handle.print_handle_error(connection_fd);
        return -1;
    }
    ProtocolMessage response_message = move(*response);
    string response_buffer = message_handle.to_string(response_message);
    view_handle.print_response_buffer(connection_fd, response_buffer);
    if (model_handle.send_message(connection_fd, response_buffer) < 0) {
        view_handle.print_send_error(connection_fd);
        return -1;
    }
    return 0;
}


ProtocolServerHandleModel::ProtocolServerHandleModel(HouseController house_controller) {
    this->house_controller = house_controller;
}

string ProtocolServerHandleModel::receive_message(int connection_fd) {
    char request_buffer[RECEIVED_BUFFER_LENGTH] = { 0 };
    if (recv(connection_fd, request_buffer, RECEIVED_BUFFER_LENGTH, 0) == 0) return "";
    return request_buffer;
}

int ProtocolServerHandleModel::send_message(int connection_fd, string response_buffer) {
    if (send(connection_fd, response_buffer.data(), response_buffer.length(), 0) == 0) return -1;
    return 0;
}

optional<ProtocolMessage> ProtocolServerHandleModel::handle_request(int connection_fd, ProtocolMessage message) {
    int request_count = message.request_count;
    if (request_count == 0) return nullopt;
    vector<MessageResponse> responses = vector<MessageResponse>();
    vector<MessageRequest> requests = message.requests;
    for (int i = 0; i < request_count; i++) {
        MessageRequest request = requests.at(i);
        switch (request.request_code) {
            case MessageRequestCode::SET_PET_FOOD: {
                house_controller.add_pet_food(connection_fd, request.request_payload);
                break;
            } case MessageRequestCode::SET_HEATING_STATUS: {
                house_controller.set_heating_status(connection_fd, request.request_payload);
                break;
            } case MessageRequestCode::SET_DOORS_STATUS: {
                house_controller.set_doors_status(connection_fd, request.request_payload);
                break;
            } case MessageRequestCode::GET_PET_FOOD_STATUS: {
                bool is_pet_food_present = house_controller.is_pet_food_present(connection_fd);
                MessageResponse response = MessageResponse();
                response.response_code = MessageResponseCode::RES_PET_FOOD_STATUS;
                response.response_payload = is_pet_food_present;
                responses.push_back(response);
                break;
            } case MessageRequestCode::GET_TEMPERATURE: {
                double temperature = house_controller.get_temperature(connection_fd);
                MessageResponse response = MessageResponse();
                response.response_code = MessageResponseCode::RES_TEMPERATURE;
                response.response_payload = temperature;
                responses.push_back(response);
                break;
            } default: {
                return nullopt;
            }
        }
    }
    ProtocolMessage response_message = ProtocolMessage();
    response_message.transaction_id = message.transaction_id;
    response_message.message_code = MessageCode::RESPONSE;
    response_message.request_count = message.request_count;
    response_message.response_count = responses.size();
    response_message.requests = message.requests;
    response_message.responses = responses;
    return response_message;
}


void ProtocolServerHandleView::print_request_buffer(int connection_fd, string request_buffer) {
    cout << "Connection fd " << connection_fd << ". Received request buffer from client " << request_buffer << "\n";
}

void ProtocolServerHandleView::print_response_buffer(int connection_fd, string response_buffer) {
    cout << "Connection fd " << connection_fd << ". Sending respone buffer to client " << response_buffer << "\n";
}

void ProtocolServerHandleView::print_receive_error(int connection_fd) {
    cout << "Connection fd " << connection_fd << ". Error receiving message\n";
}

void ProtocolServerHandleView::print_send_error(int connection_fd) {
    cout << "Connection fd " << connection_fd << ". Error sending message\n";
}

void ProtocolServerHandleView::print_handle_error(int connection_fd) {
    cout << "Connection fd " << connection_fd << ". Error handling request - no requests given\n";
}
