#include "message_info.h"

MessageInfo::MessageInfo() {
    this->endIdx = -1;
    this->startIdx = -1;
}

bool MessageInfo::valid() const {
    return this->startIdx != -1 && this->endIdx != -1;
}