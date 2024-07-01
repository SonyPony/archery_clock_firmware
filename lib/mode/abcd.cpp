#include "abcd.h"
#include <stdlib.h>
#include <stdio.h> // TODO delete
#include <lib/mode/base.h>
#include <lib/logging/logging.h>

AbcdModeData::AbcdModeData() : BaseModeData()
{
}

bool AbcdModeData::nextStep()
{
    const bool alreadyMovedToNextStep = BaseModeData::nextStep();
    if (alreadyMovedToNextStep)
        return true;

    if (!this->running()) // not running -> run; take turn from previous round
    {
        this->setRunning(true);
        this->m_currentTime = this->initializationData().time_per_round;

        // handle that there is even number of training rounds -> start the competition always with Ab round
        if (this->roundInfo().isFirstCompetitionRound())
        {
            this->m_currentTurn = AbTurn;
        }
    }

    else // if it's running, set next turn (turn overflow is handled in that method)
    {
        this->nextTurn();
    }
    return true;
}

bool AbcdModeData::handleSecTick()
{
    const bool alreadyHandledSecTick = BaseModeData::handleSecTick();
    if (alreadyHandledSecTick)
        return true;

    this->setCurrentTimer(&this->m_currentTime);

    if (this->currentTime() == 0)
        this->nextTurn();
    return true;
}

void AbcdModeData::nextTurn()
{
    if(this->m_currentTurnThisRound >= this->initializationData().turns_per_round)
    {
        this->m_currentTurnThisRound = 1;
        // turn is not set, because the last one is starting in the next round
        this->setRunning(false);
    }

    else 
    {
        this->restorePrepTimer();
        this->m_currentTurnThisRound++;
        this->m_currentTurn = (this->m_currentTurn == AbTurn) ?CdTurn :AbTurn;
        this->m_currentTime = this->initializationData().time_per_round;
    }
}

void AbcdModeData::log() const
{
    Logging::log(
        "ABCD(time: %d s, turn: %s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        this->currentTime(),
        (this->m_currentTurn == AbTurn) ? "AB" : "CD",
        (this->running()) ? "true" : "false",
        (this->isPrepTime()) ? "true" : "false",
        (this->roundInfo().isTraining()) ? "true" : "false",
        this->roundInfo().currentRound());
}

bool AbcdModeData::display(DisplayState *displayState) const
{
    if (displayState == nullptr)
        return false;

    const bool alreadyHandledDisplay = BaseModeData::display(displayState);
    if(alreadyHandledDisplay)
        return true;

    // set time
    sprintf(displayState->leftDisplay, "%3d", this->currentTime());
    // set turn
    sprintf(displayState->rightDisplay, "%s",
            (this->m_currentTurn == AbTurn) ? "AB " : "CD ");

    this->displaySemaphor(displayState);
    this->displayRound(displayState->middleDisplay);
}

void AbcdModeData::resetState(InitializationCommand initData)
{
    BaseModeData::resetState(initData);
    this->m_currentTurn = AbTurn;
    this->m_currentTurnThisRound = 1;
    this->m_currentTime = this->initializationData().time_per_round;
}