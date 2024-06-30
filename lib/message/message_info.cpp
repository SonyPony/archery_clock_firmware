#include "message_info.h"

const char* messageTypeToString(MessageType msgType) {
    switch (msgType)
    {
    case InitializationMessageType:
        return "InitializationMessageType";
    
    case StartMessageType:
        return "StartMessageType";
    case StopMessageType:
        return "StopMessageType";
    case PreviousRoundMessageType:
        return "PreviousRoundMessageType";
    case NextRoundMessageType:
        return "NextRoundMessageType";
    case PauseMessageType:
        return "PauseMessageType";
    case NextStepMessageType:
        return "NextStepMessageType";
    case BreakMessageType:
        return "BreakMessageType";
    case StateMessageType:
        return "StateMessageType";
    case RoundChangeMessageType:
        return "RoundChangeMessageType";
    case InvalidMessageType:
        return "InvalidMessageType";
    }
    return "";
}

MessageInfo::MessageInfo() {
    this->endIdx = -1;
    this->startIdx = -1;
}

bool MessageInfo::valid() const {
    return this->startIdx != -1 && this->endIdx != -1;
}