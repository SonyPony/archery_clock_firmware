#include "commands.h"
#include <stdio.h>

RoundChangeCommandInfo::RoundChangeCommandInfo(RoundInfo roundInfo) 
{
    this->m_roundInfo = roundInfo;
}

uint32_t RoundChangeCommandInfo::bytesCount() const 
{
    return RoundChangeCommandInfo::m_messageLen;
}

const char* RoundChangeCommandInfo::toBytes()
{
    this->m_buffer[0] = '<';
    this->m_buffer[1] = static_cast<char>(MessageType::RoundChangeMessageType);
    this->m_buffer[2] = (this->m_roundInfo.isTraining()) ?'1' :'0';
    sprintf(this->m_buffer + 3, "%02d", this->m_roundInfo.currentRound() % 100);
    this->m_buffer[5] = '>';
    this->m_buffer[6] = '\0';

    return this->m_buffer;
}