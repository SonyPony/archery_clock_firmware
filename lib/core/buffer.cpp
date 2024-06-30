#include "buffer.h"
#include <string.h>
#include <stdlib.h>

Buffer::Buffer(volatile char* dataBuffer, volatile uint32_t* endPointer, int bufferSize) {
    this->m_data = dataBuffer;
    this->m_capacity = bufferSize;
    this->m_dataEndIdx = endPointer;

    this->clear();
}

void Buffer::clear()
{
    if (this->m_data == nullptr)
        return;

    for (int i = 0; i < this->m_capacity; i++)
        this->m_data[i] = 0;
    *this->m_dataEndIdx = 0;
    this->m_dataStartIdx = 0;
}

uint32_t Buffer::realIdx(uint32_t relativeIdx) const {
    return (this->m_dataStartIdx + relativeIdx) % this->m_capacity;
}

uint32_t Buffer::relativeIdx(uint32_t realIdx) const {
    return this->bytesCount(this->m_dataStartIdx, realIdx);
}

char Buffer::byte(uint32_t relativeIdx) const {
    if(this->m_data == nullptr)
        return 0;

    return this->m_data[this->realIdx(relativeIdx)];
}

uint32_t Buffer::currentSize() const {
    return this->bytesCount(this->m_dataStartIdx, *this->m_dataEndIdx);
}

uint32_t Buffer::dataStartIdx() const {
    return this->m_dataStartIdx;
}

uint32_t Buffer::nextRealIdx(uint32_t readlIdx, uint32_t offset) const {
    return (readlIdx + offset) % this->m_capacity;
}

void Buffer::addByte(char byte) {
    this->m_data[*this->m_dataEndIdx] = byte;
    *this->m_dataEndIdx = this->nextRealIdx(*this->m_dataEndIdx, 1);
}

void Buffer::invalidateBytes(uint32_t bytesCount) {
    this->m_dataStartIdx = (this->m_dataStartIdx + bytesCount) % this->m_capacity;
}

uint32_t Buffer::bytesCount(uint32_t startIdx, uint32_t endIdx) const {
    const int occupiedBytes = static_cast<int>(endIdx) - static_cast<int>(startIdx);
    if(occupiedBytes < 0)    // handle edge case, when the end index is smaller than start index
        return this->m_capacity + occupiedBytes;
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