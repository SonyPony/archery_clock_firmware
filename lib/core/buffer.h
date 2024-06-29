#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

class Buffer
{
    public:
        volatile char *data;
        volatile uint32_t* data_end_idx;    // points to the element after the last byte
        uint32_t data_start_idx;
        int size;

        Buffer(volatile char* dataBuffer, volatile uint32_t* endPointer, int bufferSize);

        /**
         * @brief Returns a single byte from the buffer given an index. To retrieve data from the buffer
         * you should use this function, because the buffer works internally as circular buffer.
         * @param idx Index of the byte. It's normalized using modulo division.
         */
        char byte(uint32_t relativeIdx) const;

        /**
         * @brief buffer_clear Sets the buffer data entries to zero.
         * @param buffer Buffer that will be cleaned.
         */
        void clear();

        // TODO doc
        uint32_t realIdx(uint32_t relativeIdx) const;

        uint32_t relativeIdx(uint32_t realIdx) const;

        // TODO doc
        uint32_t currentSize() const;

        uint32_t bytesCount(uint32_t startIdx, uint32_t endIdx) const;

        // TODO doc
        void addByte(char byte);

        void invalidateBytes(uint32_t bytesCount);

        /**
         * @brief parse_int Converts a specified substring into a number. The function does not
         * check invalid characters. Thus, it could cause incorrect return value.
         * @param str Start of the parsed substring.
         * @param string_size Specifies the size of the substring
         * (number of characters) that will be processed.
         * @return Returns converted number from given substring.
         */
        int parseInt(uint32_t relativeStartIdx, uint32_t bytesCount) const;

        // TODO doc
        bool compareString(uint32_t relativeStartIdx, const char* other) const;

};


#endif // BUFFER_H
