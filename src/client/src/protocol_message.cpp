// client/protocol_message.cpp

#include "protocol_message.h"

#include <sstream>
#include <iostream>
#include <cmath>

string ProtocolMessageHandle::generate_transaction_id() {
    string id_buffer = "";
    ostringstream stream;
    srand(time(0));
    for (int i = 0; i < TRANSACTION_ID_LENGTH; i++) {
        int digit = rand() % 10;
        stream.str("");
        stream << digit;
        id_buffer.append(stream.str());
    }
    return id_buffer;
}

ProtocolMessage ProtocolMessageHandle::parse_buffer(string buffer) {
    ProtocolMessage protocol_message = ProtocolMessage();
    int current_index = 0;
    protocol_message.transaction_id = buffer.substr(current_index, TRANSACTION_ID_LENGTH);
    current_index += TRANSACTION_ID_LENGTH;
    protocol_message.message_code = (MessageCode)stoi(buffer.substr(current_index, MESSAGE_CODE_LENGTH));
    current_index += MESSAGE_CODE_LENGTH;
    protocol_message.request_count = stoi(buffer.substr(current_index, VARIABLES_CF_LENGTH));
    current_index += VARIABLES_CF_LENGTH;
    protocol_message.response_count = stoi(buffer.substr(current_index, VARIABLES_CF_LENGTH));
    current_index += VARIABLES_CF_LENGTH;
    protocol_message.requests_length = stoi(buffer.substr(current_index, VARIABLES_LF_LENGTH));
    current_index += VARIABLES_LF_LENGTH;
    protocol_message.responses_length = stoi(buffer.substr(current_index, VARIABLES_LF_LENGTH));
    current_index += VARIABLES_LF_LENGTH;
    protocol_message.message_length = stoi(buffer.substr(current_index, ML_FIELD_LENGTH));
    string mandatory_padding = MANDATORY_PADDING;
    current_index += ML_FIELD_LENGTH + mandatory_padding.length(); 
    int requests_end_index = current_index + protocol_message.requests_length;
    if (protocol_message.request_count > 0) {
        vector<MessageRequest> requests = vector<MessageRequest>();
        int requests_length = 0;
        for (int i = 0; i < protocol_message.request_count; i++) {
            MessageRequest request = MessageRequest();
            request.request_code = (MessageRequestCode)stoi(buffer.substr(current_index, VARIABLE_METADATA_FIELD_LENGTH));
            current_index += VARIABLE_METADATA_FIELD_LENGTH;
            request.request_length = stoi(buffer.substr(current_index, VARIABLE_METADATA_FIELD_LENGTH));
            current_index += VARIABLE_METADATA_FIELD_LENGTH;
            requests_length += request.request_length;
            int payload_length = request.request_length - VARIABLE_METADATA_LENGTH;
            request.request_payload = stod(buffer.substr(current_index, payload_length));
            current_index += payload_length;
            requests.push_back(request);
        }
        protocol_message.requests = requests;
        current_index = requests_end_index;
    }
    if (protocol_message.response_count > 0) {
        vector<MessageResponse> responses = vector<MessageResponse>();
        int responses_length = 0;
        for (int i = 0; i < protocol_message.response_count; i++) {
            MessageResponse response = MessageResponse();
            response.response_code = (MessageResponseCode)stoi(buffer.substr(current_index, VARIABLE_METADATA_FIELD_LENGTH));
            current_index += VARIABLE_METADATA_FIELD_LENGTH;
            response.response_length = stoi(buffer.substr(current_index, VARIABLE_METADATA_FIELD_LENGTH));
            current_index += VARIABLE_METADATA_FIELD_LENGTH;
            responses_length += response.response_length;
            int payload_length = response.response_length - VARIABLE_METADATA_LENGTH;
            response.response_payload = stod(buffer.substr(current_index, payload_length));
            current_index += payload_length;
            responses.push_back(response);
        }
        protocol_message.responses = responses;
    }
    return protocol_message;
}



string ProtocolMessageHandle::to_string(ProtocolMessage message) {
    string buffer = "";
    int request_count = message.request_count;
    int response_count = message.response_count;
    vector<MessageRequest> requests = message.requests;
    vector<MessageResponse> responses = message.responses;
    string requests_buffer = to_string_requests(request_count, requests);
    string responses_buffer = to_string_responses(response_count, responses);
    int requests_length = requests_buffer.length();
    int responses_length = responses_buffer.length();
    int message_length = requests_length + responses_length + MANDATORY_LENGTH;
    string mandatory_buffer = to_string_mandatory(message.transaction_id, message.message_code, request_count, response_count, requests_length, responses_length, message_length);    
    buffer.append(mandatory_buffer);
    buffer.append(requests_buffer);
    buffer.append(responses_buffer);
    return buffer;
}

string ProtocolMessageHandle::to_string_requests(int request_count, vector<MessageRequest> requests) {
    string requests_buffer = "";
    if (request_count > 0) {
        for (int i = 0; i < request_count; i++)
        {
            MessageRequest request = requests.at(i);
            string request_buffer = to_string_variable_chunk(request.request_code, request.request_payload);
            requests_buffer.append(request_buffer);
        }
        string padding_buffer = to_string_variable_padding(requests_buffer.length());
        requests_buffer.append(padding_buffer);
    }
    return requests_buffer;
}

string ProtocolMessageHandle::to_string_responses(int response_count, vector<MessageResponse> responses) {
    string responses_buffer = "";
    if (response_count > 0) {
        for (int i = 0; i < response_count; i++) {
            MessageResponse response = responses.at(i); 
            string response_buffer = to_string_variable_chunk(response.response_code, response.response_payload);
            responses_buffer.append(response_buffer);
        }
        string padding_buffer = to_string_variable_padding(responses_buffer.length());
        responses_buffer.append(padding_buffer);
    }
    return responses_buffer;
}


string ProtocolMessageHandle::to_string_variable_chunk(int variable_code, double variable_payload) {
    ostringstream stream;
    string item_buffer = "";
    stream << variable_code;
    item_buffer.append(stream.str());
    stream.str("");
    stream << variable_payload;
    item_buffer.append(stream.str());
    int variable_length = VARIABLE_METADATA_LENGTH + stream.str().length();
    stream.str("");
    stream << variable_length;
    string length_field_buffer = stream.str();
    int length_field_padding = VARIABLE_METADATA_FIELD_LENGTH - length_field_buffer.length();
    length_field_buffer.insert(0, length_field_padding, '0');
    item_buffer.insert(VARIABLE_METADATA_FIELD_LENGTH, length_field_buffer);
    return item_buffer;
}

string ProtocolMessageHandle::to_string_variable_padding(int variable_length) {
    string padding_buffer = "";
    double bounds_ratio = (double)variable_length / (double)HEADER_BOUNDS;
    int bounds_k = ceil(bounds_ratio);
    if (bounds_k == bounds_ratio) return padding_buffer;
    int variable_padding_length = bounds_k * HEADER_BOUNDS - variable_length;
    padding_buffer.insert(0, variable_padding_length, '0');
    return padding_buffer;
}

string ProtocolMessageHandle::to_string_mandatory(string transaction_id, MessageCode message_code, int request_count, int response_count, int requests_length, int responses_length, int message_length) {
    ostringstream stream;
    string mandatory_buffer = "";
    int padding_length;
    mandatory_buffer.append(transaction_id);
    stream << message_code;
    mandatory_buffer.append(stream.str());
    stream.str("");
    stream << request_count;
    mandatory_buffer.append(stream.str());
    stream.str("");
    stream << response_count;
    mandatory_buffer.append(stream.str());
    stream.str("");
    stream << requests_length;
    string requests_length_buffer = stream.str();
    padding_length = VARIABLES_LF_LENGTH - requests_length_buffer.length();
    requests_length_buffer.insert(0, padding_length, '0');
    mandatory_buffer.append(requests_length_buffer);
    stream.str("");
    stream << responses_length;
    string responses_length_buffer = stream.str();
    padding_length = VARIABLES_LF_LENGTH - responses_length_buffer.length();
    responses_length_buffer.insert(0, padding_length, '0');
    mandatory_buffer.append(responses_length_buffer);
    stream.str("");
    stream << message_length;
    string length_field_buffer = stream.str();
    padding_length = ML_FIELD_LENGTH - length_field_buffer.length(); 
    length_field_buffer.insert(0, padding_length, '0');
    mandatory_buffer.append(length_field_buffer);
    mandatory_buffer.append(MANDATORY_PADDING);
    return mandatory_buffer;
}
