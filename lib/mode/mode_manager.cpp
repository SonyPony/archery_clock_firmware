#include "mode_manager.h"
#include <stdlib.h>
#include <lib/message/parser.h>
#include <string.h>
#include <lib/logging/logging.h>

ModeManager::ModeManager() {
    m_currentMode = nullptr;
    this->roundChangeCallback = nullptr;
    this->modeChangeCallback = nullptr;
    this->pausedChangeCallback = nullptr;
    this->prepStartCallback = nullptr;
    this->prepEndCallback = nullptr;
    this->runningChangeCallback = nullptr;
    this->m_currentModeData.turn_type = TurnType::None_TurnType;
}

void ModeManager::initMode(InitializationCommand initData) {
    this->m_currentMode = nullptr;
    this->m_currentModeData = initData;

    switch (initData.turn_type)
    {
    case TurnType::AB_TurnType:
        this->m_currentMode = &this->m_abModeData;
        break;

    case TurnType::ABC_TurnType:
        this->m_currentMode = &this->m_abcModeData;
        break;

    case TurnType::ABCD_TurnType:
        this->m_currentMode = &this->m_abcdModeData;
        break;

    case TurnType::FinalsIndividual_TurnType:
    case TurnType::FinalsTeams_TurnType:
        this->m_currentMode = &this->m_finalsModeData;
        break;

    default:
        this->m_currentModeData.turn_type = TurnType::None_TurnType;
        break;
    } 

    if(this->m_currentMode != nullptr) {
        this->m_currentMode->roundChangeCallback = this->roundChangeCallback;
        this->m_currentMode->pausedChangeCallback = this->pausedChangeCallback;
        this->m_currentMode->prepStartCallback = this->prepStartCallback;
        this->m_currentMode->prepEndCallback = this->prepEndCallback;
        this->m_currentMode->runningChangeCallback = this->runningChangeCallback;
        this->m_currentMode->resetState(initData);
    }

    if(this->modeChangeCallback != nullptr) {
        this->modeChangeCallback(this->currentModeData());
    }
}

InitializationCommand ModeManager::currentModeData() const
{
    return this->m_currentModeData;
}

bool ModeManager::modeDataValid() const
{
    return this->m_currentMode != nullptr;
}

void ModeManager::processCommand(const BaseCommand* command)
{
    if (command == nullptr)
        return;

    const MessageType commandType = command->type;

    if (commandType == InitializationMessageType)
    {
        this->initMode(*((const InitializationCommand*)command));
    }

    else
    {
        if (!this->modeDataValid()) // check valid mode structures
            return;

        BaseModeData *modeData = this->m_currentMode;

        switch (commandType) // handle commands
        {
        case NextStepMessageType:
        case StartMessageType:
            modeData->nextStep();
            break;

        case StopMessageType:
            modeData->setRunning(false);
            break;

        case PreviousRoundMessageType:
            modeData->setPreviousRound();
            break;

        case NextRoundMessageType:
            modeData->setNextRound();
            break;

        case PauseMessageType:
            if(!modeData->paused())
                modeData->pause();
            else
                modeData->resume();
            break;

        case BreakMessageType:
            modeData->startBreak(((const BreakCommand*)command)->break_time);
            break;
        }
    }
}

BaseModeData* ModeManager::currentMode() const {
    return this->m_currentMode;
}