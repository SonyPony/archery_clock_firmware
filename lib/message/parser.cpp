#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <lib/core/buffer.h>

MessageParser::MessageParser(Buffer* buffer) {
    this->m_buffer = buffer;
}

bool MessageParser::checkMessageValid(MessageInfo msgInfo, MessageType msgType) const
{
    if (!msgInfo.valid())
        return false;

    const uint32_t msgLength = this->m_buffer->bytesCount(msgInfo.startIdx, this->m_buffer->nextRealIdx(msgInfo.endIdx, 1));    // needs to point on element after
    if(msgType == BreakMessageType)
        return msgLength == 8;   // <{Id:1}{break_time:5}>
    else if(msgType == InitializationMessageType)
        return msgLength == 14;     // <{Id:1}{training_time:3}{warning_time:3}{rounds_count:2}{mode:3}>
    return msgLength == 3;   // <{Id:1}>
}

InitializationCommand* MessageParser::parseInitializationCommand(MessageInfo msg_info, InitializationCommand *msg)
{
    if(msg == nullptr)
        return nullptr;

    const uint32_t msgContentStartIdx = msg_info.startIdx + 2; // omit id symbol (+1) and start symbol
    const uint32_t msgTurnTypeStartIdx = msgContentStartIdx + 8;

    msg->turn_type = ABCD_TurnType; // default value
    msg->turns_per_round = 2;
    msg->time_per_round = this->m_buffer->parseInt(msgContentStartIdx, 3);            // 3 decimals are reserved for time per round
    msg->warning_time = this->m_buffer->parseInt(msgContentStartIdx + 3, 3);          // 3 decimals are reserved for warning time
    msg->training_rounds_count = this->m_buffer->parseInt(msgContentStartIdx + 6, 2); // 2 decimals
    msg->prep_time = PREP_TIME;

    if (this->m_buffer->compareString(msgTurnTypeStartIdx, "AB-"))
        msg->turn_type = AB_TurnType;
    else if (this->m_buffer->compareString(msgTurnTypeStartIdx, "ABC"))
        msg->turn_type = ABC_TurnType;
    else if (this->m_buffer->compareString(msgTurnTypeStartIdx, "ABD"))
    {
        msg->turn_type = ABCD_TurnType;
        msg->turns_per_round = 2;
    }
    else if (this->m_buffer->compareString(msgTurnTypeStartIdx, "F-I"))
    {
        msg->turn_type = FinalsIndividual_TurnType;
        msg->turns_per_round = 6;
    }
    else if (this->m_buffer->compareString(msgTurnTypeStartIdx, "F-T"))
    {
        msg->turn_type = FinalsTeams_TurnType;
        msg->turns_per_round = 4;
    }
    return msg;
}

BreakCommand* MessageParser::parseBreakCommand(MessageInfo msg_info, BreakCommand *msg)
{
    if (msg == nullptr)
        return nullptr;

    const uint32_t msgContentStartIdx = msg_info.startIdx + 2; // omit id symbol (+1) + start symbol
    msg->break_time = this->m_buffer->parseInt(msgContentStartIdx, 5);                                 // 5 decimals are reserved for break time

    return msg;
}

void parse_message_info(Buffer *buffer, MessageInfo *message_info)
{
    if (buffer == NULL || message_info == NULL)
        return;

    message_info->startIdx = -1;
    message_info->endIdx = -1; // start with invalid message

    for (int i = 0; i < buffer->currentSize(); i++)
    {
        if (buffer->byte(i) == MESSAGE_START_SYMBOL)
            message_info->startIdx = buffer->realIdx(i);

        else if (buffer->byte(i) == MESSAGE_END_SYMBOL && message_info->startIdx != -1)
        {
            message_info->endIdx = buffer->realIdx(i);
            break;
        }
    }
}

MessageType parse_message_type(MessageInfo *msg_info, Buffer *buffer)
{
    if (msg_info == NULL || buffer == NULL)
        return InvalidMessageType;

    const uint32_t relativeIdxMsgStart = buffer->relativeIdx(msg_info->startIdx + 1);   // add 1 to skip start symbol
    const uint8_t raw_type = buffer->byte(relativeIdxMsgStart); 
    if (raw_type >= '0' && raw_type <= '7')
        return static_cast<MessageType>(raw_type - '0');
    else if (raw_type >= 'a' && raw_type <= 'b')
        return static_cast<MessageType>(raw_type);
    return InvalidMessageType;
}

void remove_message_from_buffer(MessageInfo *msg_info, Buffer *buffer)
{
    if (msg_info == NULL || buffer == NULL)
        return;

    if (!msg_info->valid())
        return;

    const uint32_t invalidBytes = (buffer->bytesCount(buffer->dataStartIdx(), msg_info->startIdx));
    const uint32_t messageSize = buffer->bytesCount(msg_info->startIdx, msg_info->endIdx + 1);      // needs to point on the edge of message (next element after message)
    buffer->invalidateBytes(messageSize + invalidBytes);
}

const BaseCommand* MessageParser::parseMessage()
{
    MessageInfo msg_info;
    BaseCommand *result = &m_baseCommand;

    parse_message_info(this->m_buffer, &msg_info);
    if (!msg_info.valid())
        return nullptr;

    const MessageType parsed_msg_type = parse_message_type(&msg_info, this->m_buffer);

    if(!this->checkMessageValid(msg_info, parsed_msg_type))
        return nullptr;

    result->type = parsed_msg_type;

    if (parsed_msg_type == BreakMessageType)
        result = this->parseBreakCommand(msg_info, &m_breakCommand);

    else if (parsed_msg_type == InitializationMessageType)
        result = this->parseInitializationCommand(msg_info, &m_initCommand);

    // message parsed -> remove from buffer
    remove_message_from_buffer(&msg_info, this->m_buffer);
    return result;
}