#include "buffer.h"
#include <string.h>
#include <stdlib.h>

Buffer::Buffer(volatile char* dataBuffer, volatile uint32_t* endPointer, int bufferSize) {
    this->data = dataBuffer;
    this->size = bufferSize;
    this->data_end_idx = endPointer;

    this->clear();
}

void Buffer::clear()
{
    if (this->data == nullptr)
        return;

    for (int i = 0; i < this->size; i++)
        this->data[i] = 0;
    *this->data_end_idx = 0;
    this->data_start_idx = 0;
}

uint32_t Buffer::realIdx(uint32_t relativeIdx) const {
    return (this->data_start_idx + relativeIdx) % this->size;
}

uint32_t Buffer::relativeIdx(uint32_t realIdx) const {
    return this->bytesCount(this->data_start_idx, realIdx);
}

char Buffer::byte(uint32_t relativeIdx) const {
    if(this->data == nullptr)
        return 0;

    return this->data[this->realIdx(relativeIdx)];
}

uint32_t Buffer::currentSize() const {
    return this->bytesCount(this->data_start_idx, *this->data_end_idx);
}

void Buffer::addByte(char byte) {
    this->data[*this->data_end_idx] = byte;
    *this->data_end_idx = (*this->data_end_idx + 1) % this->size;
}

void Buffer::invalidateBytes(uint32_t bytesCount) {
    this->data_start_idx = (this->data_start_idx + bytesCount) % this->size;
}

uint32_t Buffer::bytesCount(uint32_t startIdx, uint32_t endIdx) const {
    const int occupiedBytes = static_cast<int>(endIdx) - static_cast<int>(startIdx);
    if(occupiedBytes < 0)    // handle edge case, when the end index is smaller than start index
        return this->size + occupiedBytes;
    return occupiedBytes;
}

int Buffer::parseInt(uint32_t relativeStartIdx, uint32_t bytesCount) const
{
    int result = 0;
    for (int i = bytesCount - 1, decimal = 1; i >= 0; i--, decimal *= 10)
        result += (this->byte(i + relativeStartIdx) - '0') * decimal;

    return result;
}

bool Buffer::compareString(uint32_t relativeStartIdx, const char* other) const {
    const uint32_t elementCount = strlen(other);
    for(uint32_t i = 0; i < elementCount; i++) {
        if(this->byte(i + relativeStartIdx) != other[i])
            return false;
    }
    return true;
}