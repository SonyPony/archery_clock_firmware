#include "ab.h"
#include <stdio.h> // TODO delete
#include <stdlib.h>
#include <lib/logging/logging.h>

AbModeData::AbModeData(): BaseModeData() 
{}

bool AbModeData::nextStep() {
    const bool alreadyMovedToNextStep = BaseModeData::nextStep();
    if(alreadyMovedToNextStep)
        return true;

    if(!this->running())   // not running -> run;
    {
        this->setRunning(true);
        this->m_currentTime = this->initializationData().time_per_round;
        return true;
    }

    // if it's running do following
    this->setRunning(false);
    this->m_currentTime = 0;

    return true;
}

bool AbModeData::handleSecTick() {
    const bool alreadyHandledSecTick = BaseModeData::handleSecTick();
    if(alreadyHandledSecTick)
        return true;

    this->setCurrentTimer(&this->m_currentTime);
    if(this->currentTime() == 0) {
        this->setRunning(false);
        return true;
    }

    return true;
}

void AbModeData::log() const
{
    Logging::log(
        "AB(time: %d s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        this->currentTime(),
        (this->running()) ? "true" : "false",
        (this->isPrepTime()) ? "true" : "false",
        (this->roundInfo().isTraining()) ? "true" : "false",
        this->roundInfo().currentRound());
}

bool AbModeData::display(DisplayState *displayState) const
{
    if (displayState == nullptr)
        return false;

    const bool alreadyHandledDisplay = BaseModeData::display(displayState);
    if(alreadyHandledDisplay)
        return true;

    // set time
    sprintf(displayState->leftDisplay, "%3d", this->currentTime());
    // set turn
    sprintf(displayState->rightDisplay, "%s", "AB ");

    this->displaySemaphor(displayState);
    this->displayRound(displayState->middleDisplay);
    return true;
}

void AbModeData::resetState(InitializationCommand initData)
{
    BaseModeData::resetState(initData);
    this->m_currentTime = initData.time_per_round;
}