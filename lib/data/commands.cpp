#include "commands.h"
#include <stdio.h>

InitializationCommand::InitializationCommand()
: time_per_round(0), warning_time(0), training_rounds_count(0), turn_type(TurnType::None_TurnType), prep_time(0), turns_per_round(0)
{}

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
    this->m_buffer[0] = MESSAGE_START_SYMBOL;
    this->m_buffer[1] = static_cast<char>(MessageType::RoundChangeMessageType);
    this->m_buffer[2] = (this->m_roundInfo.isTraining()) ?'1' :'0';
    sprintf(this->m_buffer + 3, "%02d", this->m_roundInfo.currentRound() % 100);
    this->m_buffer[5] = MESSAGE_END_SYMBOL;
    this->m_buffer[6] = '\0';

    return this->m_buffer;
}

ModeChangeCommandInfo::ModeChangeCommandInfo(const InitializationCommand& initData)
: InitializationCommand(initData) {}

uint32_t ModeChangeCommandInfo::bytesCount() const
{
    return ModeChangeCommandInfo::m_messageLen;
}

const char* ModeChangeCommandInfo::toBytes()
{
    this->m_buffer[0] = MESSAGE_START_SYMBOL;
    this->m_buffer[1] = static_cast<char>(MessageType::CurrentModeChangeMessageType);
    sprintf(this->m_buffer + 2, "%03d", this->time_per_round % 1000);
    sprintf(this->m_buffer + 5, "%03d", this->warning_time % 1000);
    sprintf(this->m_buffer + 8, "%02d", this->training_rounds_count % 100);
    char* turnTypeBuffer = this->m_buffer + 10;

    switch (this->turn_type)
    {
    case AB_TurnType:
        sprintf(turnTypeBuffer, "AB-");
        break;

    case ABC_TurnType:
        sprintf(turnTypeBuffer, "ABC");
        break;

    case ABCD_TurnType:
        sprintf(turnTypeBuffer, "ABD");
        break;

    case FinalsIndividual_TurnType:
        sprintf(turnTypeBuffer, "F-I");
        break;

    case FinalsTeams_TurnType:
        sprintf(turnTypeBuffer, "F-T");
        break;

    default:
        sprintf(turnTypeBuffer, "---");
        break;
    }

    this->m_buffer[13] = MESSAGE_END_SYMBOL;
    this->m_buffer[14] = '\0';

    return this->m_buffer;
}

PauseChangeCommandInfo::PauseChangeCommandInfo(bool paused) 
{
    this->m_paused = paused;
}

uint32_t PauseChangeCommandInfo::bytesCount() const 
{
    return PauseChangeCommandInfo::m_messageLen;
}

const char* PauseChangeCommandInfo::toBytes() 
{
    this->m_buffer[0] = MESSAGE_START_SYMBOL;
    this->m_buffer[1] = static_cast<char>(MessageType::PauseChangeMessageType);
    this->m_buffer[2] = (this->m_paused) ?'1' :'0'; 
    this->m_buffer[3] = MESSAGE_END_SYMBOL;
    this->m_buffer[4] = '\0';

    return this->m_buffer;
}