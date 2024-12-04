// client/protocol_handle.cpp

#include "protocol_handle.h"

#include <netdb.h>
#include <iostream>

using namespace std;

ProtocolClientHandleController::ProtocolClientHandleController(ProtocolClientHandleModel model_handle, ProtocolClientHandleView view_handle, ProtocolMessageHandle message_handle) {
    this->model_handle = model_handle;
    this->view_handle = view_handle;
    this->message_handle = message_handle;
}

int ProtocolClientHandleController::handle(int client_fd, vector<MessageRequest> requests) {
    ProtocolMessage request_message = model_handle.form_request_message(message_handle, requests);
    string request_buffer = message_handle.to_string(request_message);
    view_handle.print_send_message(request_buffer);
    if (model_handle.send_message(client_fd, request_buffer) < 0) {
        view_handle.print_send_error();
        return -1;
    }
    string response_buffer = model_handle.receive_message(client_fd);
    if (response_buffer.length() == 0) {
        view_handle.print_receive_error();
        return -1;
    }
    view_handle.print_receive_message(response_buffer);
    ProtocolMessage response_message = message_handle.parse_buffer(response_buffer);
    view_handle.print_server_responses(response_message.responses);
    return 0;
}
  
ProtocolMessage ProtocolClientHandleModel::form_request_message(ProtocolMessageHandle message_handle, vector<MessageRequest> requests) {
    ProtocolMessage message = ProtocolMessage();
    message.message_code = MessageCode::REQUEST;
    message.transaction_id = message_handle.generate_transaction_id();
    message.request_count = requests.size();
    message.response_count = 0;
    message.requests = requests;
    return message;
}

string ProtocolClientHandleModel::receive_message(int client_fd) {
    char buffer[RECEIVED_BUFFER_LENGTH] = { 0 };
    if (recv(client_fd, buffer, RECEIVED_BUFFER_LENGTH, 0) == 0) return "";
    return buffer;
}

int ProtocolClientHandleModel::send_message(int client_fd, string request_buffer) {
    if (send(client_fd, request_buffer.data(), request_buffer.length(), 0) == 0) return -1;
    return 0;
}


void ProtocolClientHandleView::print_send_message(string request_buffer) {
    cout << "Sending request buffer to server: " << request_buffer << "\n";
}

void ProtocolClientHandleView::print_receive_message(string response_buffer) {
    cout << "Received response buffer from server: " << response_buffer << "\n";
}

void ProtocolClientHandleView::print_server_responses(vector<MessageResponse> responses) {
    int responses_vector_length = responses.size();
    if (responses_vector_length == 0) {
        cout << "Client requests do not contain instructions to server to make responses\n";
        return;
    }
    cout << "Printing server responses...\n";
    for (int i = 0; i < responses_vector_length; i++) {
        MessageResponse response = responses.at(i);
        switch(response.response_code) {
            case MessageResponseCode::RES_TEMPERATURE: {
                cout << "Current temperature: " << response.response_payload << "\n";
                break;
            }
            case MessageResponseCode::RES_PET_FOOD_STATUS: {
                string pet_food_status_buffer = response.response_payload ? "present" : "absent";
                cout << "Pet food is " << pet_food_status_buffer << "\n";
                break;
            }
            default: {
                cout << "Unknown response from server\n";
                break;
            }
        }
    }
}

void ProtocolClientHandleView::print_send_error() {
    cout << "Error sending request to server\n";
}

void ProtocolClientHandleView::print_receive_error() {
    cout << "Error receiving response from server\n";
}