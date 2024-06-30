#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <lib/peripheral/display.h>
#include <lib/data/data_model.h>
#include <lib/logging/logging.h>


RoundInfo::RoundInfo(int trainingRoundsCount)
{
    this->m_trainingRoundsCount = trainingRoundsCount;
    this->m_currentRound = 1;
    this->m_isTraining = (trainingRoundsCount > 0);
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

BaseModeData::BaseModeData(InitializationCommand initData) {
    this->m_currentTimer = nullptr;

    this->resetState(initData);
}

void BaseModeData::resetState(InitializationCommand initData) {
    this->m_running = false;
    this->m_paused = false;
    this->m_initData = initData;
    this->m_isBreak = false;
    this->m_breakTimer = 0;
    this->m_roundInfo = RoundInfo(initData.training_rounds_count);

    this->restorePrepTimer();
}

void BaseModeData::restorePrepTimer() {
    this->m_prepTimer = this->m_initData.prep_time;
    this->m_currentTimer = &this->m_prepTimer;
}

bool BaseModeData::running() const {
    return this->m_running;
}

int BaseModeData::currentTime() const {
    return *(this->m_currentTimer);
}

void BaseModeData::decrementCurrentTime()
{
    *(this->m_currentTimer) -= 1;
}

void BaseModeData::setRunning(bool running) {
    this->m_running = running;
    if(this->m_running)
        this->restorePrepTimer();
    else
        this->setNextRound();
}

bool BaseModeData::isPrepTime() const
{
    return this->m_prepTimer > 0;
}

bool BaseModeData::isWarningTime() const
{
    if (!this->isPrepTime())
        return this->currentTime() <= this->m_initData.warning_time;
    return false;
}

void BaseModeData::setNextRound() {
    this->m_roundInfo.setNextRound();
}

void BaseModeData::setPreviousRound() {
    this->m_roundInfo.setPreviousRound();
}

void BaseModeData::displayRound(char *targetBuffer) const
{
    if (targetBuffer == nullptr)
        return;

    // there are two digits for round
    if (this->m_roundInfo.isTraining())
    {
        targetBuffer[0] = 'P';
        targetBuffer[1] = this->m_roundInfo.currentRound() % 10 + '0';
        return;
    }

    // competition round
    sprintf(targetBuffer, "%2d", this->m_roundInfo.currentRound() % 100);
}

bool BaseModeData::currentTimerIsPrepTimer() const 
{
    return this->m_currentTimer == &this->m_prepTimer;
}

void BaseModeData::displaySemaphor(DisplayState *displayState) const
{
    if (displayState == nullptr)
        return;

    // set sempahor color
    if (this->isPrepTime())
        displayState->semaphorDisplay = SemaphorDisplayRed;
    else if (this->isWarningTime())
        displayState->semaphorDisplay = SemaphorDisplayOrange;
    else
        displayState->semaphorDisplay = SemaphorDisplayGreen;
}

void BaseModeData::displayBreak(DisplayState *displayState) const
{
    if (displayState == nullptr)
        return;

    const int current_time = this->m_breakTimer;
    const int minutes = current_time / 60;
    const int seconds = current_time % 60;
    displayState->semaphorDisplay = (current_time % 2) ? SemaphorDisplayOrange : SemaphorDisplayEmpty;
    
    sprintf(displayState->middleDisplay, "%s", "--");
    sprintf(displayState->leftDisplay, "%3d", minutes);
    sprintf(displayState->rightDisplay, "%3d", seconds);
}

void BaseModeData::pause()
{
    this->m_paused = true;
    this->m_running = false;
}

void BaseModeData::resume()
{
    this->m_paused = false;
    this->m_running = true;
}

void BaseModeData::startBreak(int breakTime)
{
    if (this->running()) // do nothing if the mode is running (can't insert a break)
        return;

    this->m_isBreak = true;
    this->m_breakTimer = breakTime;
}