#include "parser.h"
#include <core.h>
#include <string.h>

bool message_info_valid(MessageInfo *msg_info)
{
    if (msg_info == NULL)
        return false;

    return msg_info->startIdx != -1 && msg_info->endIdx != -1;
}

bool break_message_valid(MessageInfo *msg_info)
{
    if (msg_info == NULL)
        return false;

    return msg_info->endIdx - msg_info->startIdx + 1 == 8;
}

bool initialization_message_valid(MessageInfo *msg_info)
{
    if (msg_info == NULL)
        return false;

    return msg_info->endIdx - msg_info->startIdx + 1 == 14;
}

void parse_initialization_data(Buffer *buffer, MessageInfo *msg_info, InitializationData *msg)
{
    if (buffer == NULL || msg_info == NULL || msg == NULL)
        return;

    const volatile uint8_t *raw_buffer = buffer->data + msg_info->startIdx + 2; // omit id symbol (+1) and start symbol
    const volatile uint8_t *raw_turn_type = raw_buffer + 8;

    msg->turn_type = ABCD_TurnType; // default value
    msg->turns_per_round = 1;
    msg->time_per_round = parse_int(raw_buffer, 3);            // 3 decimals are reserved for time per round
    msg->warning_time = parse_int(raw_buffer + 3, 3);          // 3 decimals are reserved for warning time
    msg->training_rounds_count = parse_int(raw_buffer + 6, 2); // 2 decimals

    if (strncmp(raw_turn_type, "AB-", 3) == 0)
        msg->turn_type = AB_TurnType;
    else if (strncmp(raw_turn_type, "ABC", 3) == 0)
        msg->turn_type = ABC_TurnType;
    else if (strncmp(raw_turn_type, "ABD", 3) == 0)
    {
        msg->turn_type = ABCD_TurnType;
        msg->turns_per_round = 2;
    }
    else if (strncmp(raw_turn_type, "F-I", 3) == 0)
    {
        msg->turn_type = FinalsIndividual_TurnType;
        msg->turns_per_round = 6;
    }
    else if (strncmp(raw_turn_type, "F-T", 3) == 0)
    {
        msg->turn_type = FinalsTeams_TurnType;
        msg->turns_per_round = 4;
    }
}

void parse_break_data(Buffer *buffer, MessageInfo *msg_info, BreakData *msg)
{
    if (buffer == NULL || msg_info == NULL || msg == NULL)
        return;

    const volatile uint8_t *raw_buffer = buffer->data + msg_info->startIdx + 2; // omit id symbol (+1)
    msg->break_time = parse_int(raw_buffer, 5);                                 // 5 decimals are reserved for break time
}

void parse_message_info(Buffer *buffer, MessageInfo *message_info)
{
    if (buffer == NULL || message_info == NULL)
        return;

    message_info->startIdx = -1;
    message_info->endIdx = -1; // start with invalid message

    for (int i = 0; i < *buffer->data_end_idx; i++)
    {
        if (buffer->data[i] == MESSAGE_START_SYMBOL)
            message_info->startIdx = i;

        else if (buffer->data[i] == MESSAGE_END_SYMBOL && message_info->startIdx != -1)
        {
            message_info->endIdx = i;
            break;
        }
    }
}

MessageType parse_message_type(MessageInfo *msg_info, Buffer *buffer)
{
    if (msg_info == NULL || buffer == NULL)
        return InvalidMessageType;

    const uint8_t raw_type = buffer->data[msg_info->startIdx + 1]; // add 1 to skip start symbol
    if (raw_type >= '0' && raw_type <= '7')
        return (MessageType)(raw_type - '0');
    return (MessageType)raw_type;
}

void remove_message_from_buffer(MessageInfo *msg_info, Buffer *buffer)
{
    if (msg_info == NULL || buffer == NULL)
        return;

    if (!message_info_valid(msg_info))
        return;

    memmove(buffer->data, buffer->data + msg_info->endIdx + 1, buffer->size - msg_info->endIdx - 1);
    *buffer->data_end_idx = (*buffer->data_end_idx) - (msg_info->endIdx + 1);
}
