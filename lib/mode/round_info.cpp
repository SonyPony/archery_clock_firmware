#include "round_info.h"

RoundInfo::RoundInfo(int trainingRoundsCount)
{
    this->m_trainingRoundsCount = trainingRoundsCount;
    this->m_currentRound = 1;
    this->m_isTraining = (trainingRoundsCount > 0);
}

int RoundInfo::currentRound() const {
    return this->m_currentRound;
}

bool RoundInfo::isTraining() const {
    return this->m_isTraining;
}

bool RoundInfo::isFirstCompetitionRound() const 
{
    return this->m_currentRound == 1 && !this->m_isTraining;
}

void RoundInfo::setPreviousRound()
{
    this->m_currentRound--; // decrese the current round

    // switch back to training rounds
    if (!this->m_isTraining && this->m_currentRound <= 0)
    {
        this->m_currentRound = this->m_trainingRoundsCount;
        this->m_isTraining = true;
    }

    // handle underflow of training rounds
    if (this->m_isTraining && this->m_currentRound <= 0)
        this->m_currentRound = 1;
}

void RoundInfo::setNextRound()
{
    this->m_currentRound++; // increase current round

    // end of training rounds
    if (this->m_isTraining && this->m_currentRound > this->m_trainingRoundsCount)
    {
        this->m_currentRound = 1;
        this->m_isTraining = false;
    }

    // handle maximum number of rounds (99)
    if (this->m_currentRound > 99)
        this->m_currentRound = 99;
}