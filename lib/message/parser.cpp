#include "parser.h"
#include <lib/core/core.h>
#include <string.h>
#include <stdlib.h>
#include <lib/core/buffer.h>
#include <lib/core/core.h>

bool message_info_valid(MessageInfo *msg_info)
{
    if (msg_info == NULL)
        return false;

    return msg_info->startIdx != -1 && msg_info->endIdx != -1 && msg_info->startIdx < msg_info->endIdx;
}

bool break_message_valid(MessageInfo *msg_info, Buffer* buffer)
{
    if (msg_info == NULL)
        return false;

    return buffer->bytesCount(msg_info->startIdx, msg_info->endIdx) == 8;   // <{Id:1}{break_time:5}>
}

bool initialization_message_valid(MessageInfo *msg_info, Buffer* buffer)
{
    if (msg_info == NULL)
        return false;

    return buffer->bytesCount(msg_info->startIdx, msg_info->endIdx) == 14;     // <{Id:1}{training_time:3}{warning_time:3}{rounds_count:2}{mode:3}>
}

void parse_initialization_data(Buffer *buffer, MessageInfo *msg_info, InitializationCommand *msg)
{
    if (buffer == NULL || msg_info == NULL || msg == NULL)
        return;

    const uint32_t msgContentStartIdx = msg_info->startIdx + 2; // omit id symbol (+1) and start symbol
    const uint32_t msgTurnTypeStartIdx = msgContentStartIdx + 8;

    msg->turn_type = ABCD_TurnType; // default value
    msg->turns_per_round = 2;
    msg->time_per_round = buffer->parseInt(msgContentStartIdx, 3);            // 3 decimals are reserved for time per round
    msg->warning_time = buffer->parseInt(msgContentStartIdx + 3, 3);          // 3 decimals are reserved for warning time
    msg->training_rounds_count = buffer->parseInt(msgContentStartIdx + 6, 2); // 2 decimals
    msg->prep_time = PREP_TIME;

    if (buffer->compareString(msgTurnTypeStartIdx, "AB-"))
        msg->turn_type = AB_TurnType;
    else if (buffer->compareString(msgTurnTypeStartIdx, "ABC"))
        msg->turn_type = ABC_TurnType;
    else if (buffer->compareString(msgTurnTypeStartIdx, "ABD"))
    {
        msg->turn_type = ABCD_TurnType;
        msg->turns_per_round = 2;
    }
    else if (buffer->compareString(msgTurnTypeStartIdx, "F-I"))
    {
        msg->turn_type = FinalsIndividual_TurnType;
        msg->turns_per_round = 6;
    }
    else if (buffer->compareString(msgTurnTypeStartIdx, "F-T"))
    {
        msg->turn_type = FinalsTeams_TurnType;
        msg->turns_per_round = 4;
    }
}

void parse_break_data(Buffer *buffer, MessageInfo *msg_info, BreakCommand *msg)
{
    if (buffer == NULL || msg_info == NULL || msg == NULL)
        return;

    const uint32_t msgContentStartIdx = msg_info->startIdx + 2; // omit id symbol (+1) + start symbol
    msg->break_time = buffer->parseInt(msgContentStartIdx, 5);                                 // 5 decimals are reserved for break time
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
    return static_cast<MessageType>(raw_type);
}

void remove_message_from_buffer(MessageInfo *msg_info, Buffer *buffer)
{
    if (msg_info == NULL || buffer == NULL)
        return;

    if (!message_info_valid(msg_info))
        return;

    buffer->invalidateBytes(buffer->bytesCount(msg_info->startIdx, msg_info->endIdx));
    //memmove(buffer->data, buffer->data + msg_info->endIdx + 1, buffer->size - msg_info->endIdx - 1);
    //*buffer->data_end_idx = (*buffer->data_end_idx) - (msg_info->endIdx + 1);
}

void *parse_message(Buffer *buffer, BreakCommand *break_command, InitializationCommand *init_command, BaseCommand *base_command)
{
    if (buffer == NULL)
        return NULL;

    MessageInfo msg_info;
    void *result = NULL;
    parse_message_info(buffer, &msg_info);
    if (!message_info_valid(&msg_info))
        return NULL;

    const MessageType parsed_msg_type = parse_message_type(&msg_info, buffer);

    if (parsed_msg_type == BreakMessageType)
    {
        if (break_message_valid(&msg_info, buffer))
        {
            if (break_command == NULL)
                return NULL;
            parse_break_data(buffer, &msg_info, break_command);
            result = break_command;
        }
    }

    else if (parsed_msg_type == InitializationMessageType)
    {
        if (initialization_message_valid(&msg_info, buffer))
        {
            if (init_command == NULL)
                return NULL;
            parse_initialization_data(buffer, &msg_info, init_command);
            init_command->prep_time = PREP_TIME;

            result = init_command;
        }
    }

    else    // NextStepMessageType, StartMessageType, StopMessageType, PreviousRoundMessageType, NextRoundMessageType, PauseMessageType
    {
        if (base_command == NULL)
            return NULL;
        result = base_command;
    }

    ((BaseCommand *)result)->type = parsed_msg_type;
    // message parsed -> remove from buffer
    remove_message_from_buffer(&msg_info, buffer);
    return (void *)result;
}