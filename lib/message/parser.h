#ifndef PARSER_H
#define PARSER_H

#include "message_info.h"
#include <lib/core/buffer.h>
#include <lib/data/data_model.h>

#define MESSAGE_START_SYMBOL '<'
#define MESSAGE_END_SYMBOL '>'

class MessageParser {
    private:
        Buffer* m_buffer;
        BreakCommand m_breakCommand;
        InitializationCommand m_initCommand;
        BaseCommand m_baseCommand;

    protected:
        /**
         * @brief parseInitializationCommand It extracts initialization data from the buffer
         * based on the message info.
         * @param buffer Message buffer.
         * @param msg_info Message info that contains location of the initialization message.
         * @param msg Location where the parsed initialization data will be stored.
         */
        InitializationCommand* parseInitializationCommand(MessageInfo msgInfo, InitializationCommand *msg);

        /**
         * @brief parseBreakCommand It extracts the break data from the buffer based on the
         * message info.
         * @param buffer Message buffer.
         * @param msg_info Message info that contains location of the initialization message.
         * @param msg Location where the parsed break data will be stored.
         */
        BreakCommand* parseBreakCommand(MessageInfo msgInfo, BreakCommand *msg);

        /**
         * @brief parse_message_info Parse the first found valid message info from the message buffer.
         * @param buffer Message buffer.
         * @return Location where message info that points to the location of the buffer will be stored.
         */
        MessageInfo parseMessageInfo() const;

        /**
         * @brief checkMessageValid Controls whether the size of a message is correct.
         * @param msg_info Controlled message info.
         * @return Returns true if the message has correct size, otherwise it returns false.
         */
        bool checkMessageValid(MessageInfo msgInfo, MessageType msgType) const;

        /**
         * @brief parse_message_type Parses the message type of the raw message.
         * @param msg_info Message information of the parsed message.
         * @param buffer Message buffer.
         * @return Parsed message type.
         */
        MessageType parseMessageType(MessageInfo msgInfo) const;

        /**
         * @brief remove_message_from_buffer Removes the message specified by message info from
         * the buffer (the buffer size is not changed). All data that followed the removed message
         * will be shifted to the "left". So the buffer could work as a queue.
         * @param msg_info Message info that specifies a message that will be deleted.
         * @param buffer Message buffer.
         */
        void removeMessageFromBuffer(MessageInfo msgInfo);

    public:
        MessageParser(Buffer* buffer);

    /**
     * @brief parse_message It parses a message from the buffer. It allocates the space on the heap as well.
     * @return Returns a pointer to Command or InitializationCommand or BreakCommand. Commands have to be freed after processing.
     * Or they will cause memory leaks.
     */
    const BaseCommand* parseMessage();
};

#endif // PARSER_H
