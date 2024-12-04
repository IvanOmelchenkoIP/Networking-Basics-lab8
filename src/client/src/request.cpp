// client/request.cpp

#include "request.h"

#include <unistd.h>
#include <sstream>
#include <iostream>
#include <bits/stdc++.h>

void show_input_error() {
    cout << "Cannot extract requests from empty input...\n";
}

void show_value_error() {
    cout << "A value that is equal to 1 or 0 should be given to requests to change door or heating status and a positive float to change pet food amount...\n";
}

void show_get_error() {
    cout << "No values are supported after requests to get temperature or pet food status...\n";
}

void show_option_error(string option) {
    cout << "Option " << option << " not supported...\n";
}

void show_used_error(string option) {
    cout << "Option " << option << " was used several times. Please use every option once...\n";
}

void show_request_prompt() {
    cout << "Input options for request to server:\n"
            << "- To set heating status print: -h {0 to disable, 1 to enable}\n"
            << "- To set doors status print: -d {0 to close, 1 to open}\n"
            << "- To add pet food print: -fa {a positive floating point value}\n"
            << "- To get temperature print: -t\n"
            << "- To find out if pet food is present print: -fs\n"
            << "To separate request and value or several requests use a single space. Include single request of any of the above types.\n"
            << "Enter request > ";
}

string get_user_input() {
    const int BUFFER_SIZE = 64;
    char input_buffer[BUFFER_SIZE];
    cin.getline(input_buffer, sizeof(input_buffer));
    return input_buffer;
}

bool handle_change_request_flag(bool change_request_flag) {
    if (change_request_flag == true) {
        show_value_error();
        return false;
    }
    return true;
}

bool handle_option_once(vector<string> used_options, string option) {
    if (count(used_options.begin(), used_options.end(), option) > 0) {
        show_used_error(option);
        return false;
    }
    return true;
}

optional<vector<MessageRequest>> parse_user_input(string input) {
    const string SET_HEATING_OPT = "-h";
    const string SET_DOORS_OPT = "-d";
    const string ADD_PET_FOOD_OPT = "-fa";
    const string GET_TEMPERATURE_OPT = "-t";
    const string GET_PET_FOOD_STATUS_OPT = "-fs";

    if (input.length() == 0) {
        show_input_error();
        return nullopt;
    } 

    istringstream stream(input);
    string chunk;
    const char delimeter = ' ';
    vector<MessageRequest> requests = vector<MessageRequest>();
    vector<string> used_options = vector<string>();
    MessageRequest request;
    bool change_request_flag = false;
    bool get_request_flag = false;
    while (getline(stream, chunk, delimeter)) {
        if (chunk == SET_HEATING_OPT) {
            if (handle_change_request_flag(change_request_flag) == false) return nullopt;
            if (handle_option_once(used_options, chunk) == false) return nullopt;
            get_request_flag = false;
            change_request_flag = true;
            request = MessageRequest();
            request.request_code = MessageRequestCode::SET_HEATING_STATUS;
            used_options.push_back(chunk);
        } else if (chunk == SET_DOORS_OPT) {
            if (handle_change_request_flag(change_request_flag) == false) return nullopt;
            if (handle_option_once(used_options, chunk) == false) return nullopt;
            get_request_flag = false;
            change_request_flag = true;
            request = MessageRequest();
            request.request_code = MessageRequestCode::SET_DOORS_STATUS;
            used_options.push_back(chunk);
        } else if (chunk == ADD_PET_FOOD_OPT) {
            if (handle_change_request_flag(change_request_flag) == false) return nullopt;
            if (handle_option_once(used_options, chunk) == false) return nullopt;
            get_request_flag = false;
            change_request_flag = true;
            request = MessageRequest();
            request.request_code = MessageRequestCode::SET_PET_FOOD;
            used_options.push_back(chunk);
        } else if (chunk == GET_TEMPERATURE_OPT) {
            if (handle_change_request_flag(change_request_flag) == false) return nullopt;
            if (handle_option_once(used_options, chunk) == false) return nullopt;
            get_request_flag = true;
            request = MessageRequest();
            request.request_code = MessageRequestCode::GET_TEMPERATURE;
            request.request_payload = 0;
            requests.push_back(request);
            used_options.push_back(chunk);
        } else if (chunk == GET_PET_FOOD_STATUS_OPT) {
            if (handle_change_request_flag(change_request_flag) == false) return nullopt;
            if (handle_option_once(used_options, chunk) == false) return nullopt;
            get_request_flag = true;
            request = MessageRequest();
            request.request_code = MessageRequestCode::GET_PET_FOOD_STATUS;
            request.request_payload = 0;
            requests.push_back(request);
            used_options.push_back(chunk);
        } else {
            if (change_request_flag == true) {
                change_request_flag = false;
                try {
                    double payload = stod(chunk);
                    bool is_positive_float = payload > 0;
                    if (request.request_code == MessageRequestCode::SET_DOORS_STATUS || request.request_code == MessageRequestCode::SET_HEATING_STATUS) {
                        bool is_boolean = payload == 0 || payload == 1;
                        if (is_boolean == false) throw exception();
                    } else {
                        if (payload <= 0) throw exception();
                    }
                    request.request_payload = payload;
                } catch(exception ex) {
                    show_value_error();
                    return nullopt;
                }
                requests.push_back(request);
            } else if (get_request_flag == true) {
                show_get_error();
                return nullopt;
            } else {
                show_option_error(chunk);
                return nullopt;
            }
        }
    }
    if (change_request_flag == true) {
        show_value_error();
        return nullopt;
    }
    return requests;
}

optional<vector<MessageRequest>> get_requests() {
    show_request_prompt();
    string input = get_user_input();
    return parse_user_input(input);
}
