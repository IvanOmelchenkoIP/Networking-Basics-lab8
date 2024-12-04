// client/request.h

#ifndef REQUEST_H
#define REQUEST_H

#include "protocol_message.h"

#include <optional>

optional<vector<MessageRequest>> get_requests();

#endif