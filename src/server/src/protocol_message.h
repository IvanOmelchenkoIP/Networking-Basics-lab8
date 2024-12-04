// protocol_message.h

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <vector>
#include <string>

using namespace std;

#define MAX_MESSAGE_LENGTH 96

enum MessageCode {
    REQUEST = 11,
    RESPONSE = 21,
};

enum MessageRequestCode {
    SET_DOORS_STATUS = 11,
    SET_HEATING_STATUS = 12,
    SET_PET_FOOD = 13,
    GET_TEMPERATURE = 21,
    GET_PET_FOOD_STATUS = 22
};

enum MessageResponseCode {
    RES_TEMPERATURE = 21,
    RES_PET_FOOD_STATUS = 22
};

struct MessageRequest {
    MessageRequestCode request_code;
    int request_length;
    double request_payload;
};

struct MessageResponse {
    MessageResponseCode response_code;
    int response_length;
    double response_payload;
};

struct ProtocolMessage {
    string transaction_id;
    MessageCode message_code;
    int request_count;
    int response_count;
    int requests_length;
    int responses_length;
    int message_length;
    vector<MessageRequest> requests;
    vector<MessageResponse> responses;
};

class ProtocolMessageHandle {

private:
    static constexpr int MANDATORY_LENGTH = 32;
    static constexpr int HEADER_BOUNDS = 32;

    static constexpr int TRANSACTION_ID_LENGTH =  16;
    static constexpr int MESSAGE_CODE_LENGTH = 2;
    static constexpr int VARIABLES_CF_LENGTH = 1;
    static constexpr int VARIABLES_LF_LENGTH = 2;
    static constexpr int ML_FIELD_LENGTH = 3;
    static constexpr int VARIABLE_METADATA_LENGTH = 4;
    static constexpr int VARIABLE_METADATA_FIELD_LENGTH = 2;

    static constexpr char * MANDATORY_PADDING = "00000";

    string to_string_requests(int request_count, vector<MessageRequest> requests);
    string to_string_responses(int response_count, vector<MessageResponse> responses);
    string to_string_variable_chunk(int variable_code, double variable_payload);
    string to_string_variable_padding(int variable_length);
    string to_string_mandatory(string transaction_id, MessageCode message_code, int request_count, int response_count, int requests_length, int responses_length, int message_length);

public:
    ProtocolMessage parse_buffer(string buffer);
    string to_string(ProtocolMessage message);

    string generate_transaction_id();
};

#endif
