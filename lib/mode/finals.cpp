#include "finals.h"
#include <stdlib.h>
#include <stdio.h> // TODO delete
#include <lib/data/data_model.h>
#include <lib/logging/logging.h>

FinalsModeData::FinalsModeData(): BaseModeData()
{}

void FinalsModeData::nextTurn()
{
    if (this->m_currentTurnThisRound >= this->initializationData().turns_per_round)
    {
        this->endRound();
    }

    else
    {
        this->m_currentTurnThisRound++;
        if (this->m_currentTurn == FinalsLeftTurn)
        {
            this->m_currentTurn = FinalsRightTurn;
            this->setCurrentTimer(&this->m_rightCurrentTime);
            if (this->m_resetTimeAfterTurn)
            {
                this->m_leftCurrentTime = this->initializationData().time_per_round;
            }
        }
        else
        {
            this->m_currentTurn = FinalsLeftTurn;
            this->setCurrentTimer(&this->m_leftCurrentTime);
            if (this->m_resetTimeAfterTurn)
            {
                this->m_rightCurrentTime = this->initializationData().time_per_round;
            }
        }
    }
}

bool FinalsModeData::nextStep()
{
    const bool alreadyMovedToNextStep = BaseModeData::nextStep();
    if (alreadyMovedToNextStep)
        return true;

    if (!this->running())   // not running -> run;
    {                                                 
        this->setRunning(true); // start of another round (turn is already prepared from the last call)
        this->m_leftCurrentTime = this->initializationData().time_per_round;
        this->m_rightCurrentTime = this->initializationData().time_per_round;

        // handle that there is even number of training rounds -> start the competition always with Ab round
        if (this->roundInfo().isFirstCompetitionRound())
        {
            this->m_currentTurn = FinalsLeftTurn;
        }
    }

    else
    { // if it's running, set next turn (turn overflow is handled in that method)
        this->nextTurn();
    }
    return true;
}

bool FinalsModeData::handleSecTick()
{
    const bool alreadyHandledSecTick = BaseModeData::handleSecTick();
    if (alreadyHandledSecTick)
        return true;

    if (this->currentTimerIsPrepTimer())
    {
        this->setCurrentTimer(&this->m_leftCurrentTime);
    }

    if (this->currentTime() == 0)
    {
        if (this->m_currentTurn == FinalsLeftTurn)
        {
            this->nextStep();
        }

        else
        {
            this->endRound();
        }
    }
    return true;
}

void FinalsModeData::log() const
{
    Logging::log(
        "Finals(time: %d s, turn: %s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        this->currentTime(),
        (this->m_currentTurn == FinalsLeftTurn) ? "L" : "R",
        (this->running()) ? "true" : "false",
        (this->isPrepTime()) ? "true" : "false",
        (this->roundInfo().isTraining()) ? "true" : "false",
        this->roundInfo().currentRound());
}

void FinalsModeData::resetState(InitializationCommand initData)
{
    BaseModeData::resetState(initData);
    this->endRound();
    this->m_resetTimeAfterTurn = initData.turn_type == FinalsIndividual_TurnType;
    this->m_leftCurrentTime = this->initializationData().time_per_round;
    this->m_rightCurrentTime = this->initializationData().time_per_round;
}

bool FinalsModeData::display(DisplayState *displayState) const
{
    if (displayState == nullptr)
        return false;

    const bool alreadyHandledDisplay = BaseModeData::display(displayState);
    if(alreadyHandledDisplay)
        return true;

    this->displaySemaphor(displayState);
    // set time
    sprintf(displayState->leftDisplay, "%3d", this->m_leftCurrentTime);
    sprintf(displayState->rightDisplay, "%3d", this->m_rightCurrentTime);

    // display turn
    sprintf(displayState->middleDisplay, "%s",
            (this->m_currentTurn == FinalsLeftTurn) ? "< " : " >");
}

void FinalsModeData::endRound()
{
    this->m_currentTurnThisRound = 1;
    this->m_currentTurn = FinalsLeftTurn;
    this->setRunning(false);
}
