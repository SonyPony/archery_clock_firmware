#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "message_info.h"
#include <buffer.h>
#include <data_model.h>

#define MESSAGE_START_SYMBOL '<'
#define MESSAGE_END_SYMBOL '>'

/**
 * @brief message_info_valid Controls if the message info is valid (points to valid location).
 * @param msg_info Controlled message info.
 * @return Returns true if the message info points to valid location in message buffer,
 * otherwise it returns false.
 */
bool message_info_valid(MessageInfo *msg_info);

/**
 * @brief break_message_valid Controls whether the size of break message is correct.
 * @param msg_info Controlled message info.
 * @return Returns true if the message has correct size, otherwise it returns false.
 */
bool break_message_valid(MessageInfo *msg_info);

/**
 * @brief initialization_message_valid Controls whether the size of initialization
 * message is correct.
 * @param msg_info Controlled message info.
 * @return Returns true if the message has correct size, otherwise it returns false.
 */
bool initialization_message_valid(MessageInfo *msg_info);

/**
 * @brief parse_initialization_data It extracts initialization data from the buffer
 * based on the message info.
 * @param buffer Message buffer.
 * @param msg_info Message info that contains location of the initialization message.
 * @param msg Location where the parsed initialization data will be stored.
 */
void parse_initialization_data(Buffer *buffer, MessageInfo *msg_info, InitializationCommand *msg);

/**
 * @brief parse_break_data It extracts the break data from the buffer based on the
 * message info.
 * @param buffer Message buffer.
 * @param msg_info Message info that contains location of the initialization message.
 * @param msg Location where the parsed break data will be stored.
 */
void parse_break_data(Buffer *buffer, MessageInfo *msg_info, BreakCommand *msg);

/**
 * @brief parse_message It parses a message from the buffer. It allocates the space on the heap as well.
 * @param buffer Message buffer.
 * @return Returns a pointer to Command or InitializationCommand or BreakCommand. Commands have to be freed after processing.
 * Or they will cause memory leaks.
 */
// TODO update doc
void *parse_message(Buffer *buffer, BreakCommand *break_command, InitializationCommand *init_command, BaseCommand *base_command);

/**
 * @brief parse_message_info Parse the first found valid message info from the message buffer.
 * @param buffer Message buffer.
 * @param message_info Location where message info that points to the location of the buffer will be stored.
 */
void parse_message_info(Buffer *buffer, MessageInfo *message_info);

/**
 * @brief parse_message_type Parses the message type of the raw message.
 * @param msg_info Message information of the parsed message.
 * @param buffer Message buffer.
 * @return Parsed message type.
 */
MessageType parse_message_type(MessageInfo *msg_info, Buffer *buffer);

/**
 * @brief remove_message_from_buffer Removes the message specified by message info from
 * the buffer (the buffer size is not changed). All data that followed the removed message
 * will be shifted to the "left". So the buffer could work as a queue.
 * @param msg_info Message info that specifies a message that will be deleted.
 * @param buffer Message buffer.
 */
void remove_message_from_buffer(MessageInfo *msg_info, Buffer *buffer);

#endif // PARSER_H
