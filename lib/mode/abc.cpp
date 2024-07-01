#include "abc.h"
#include <lib/mode/ab.h>
#include <stdlib.h>
#include <stdio.h> // TODO delete
#include <lib/logging/logging.h>


AbcModeData::AbcModeData(): AbModeData()
{}

bool AbcModeData::display(DisplayState* displayState) const {
    if(displayState == nullptr)
        return false;

    AbModeData::display(displayState);
    if(!this->isBreak()) {
        sprintf(displayState->rightDisplay, "%s", "ABC");    
    }
    return true;
}

void AbcModeData::log() const {
    Logging::log(
        "ABC(time: %d s, running: %s, prep: %s, training: %s, round: %d)\n\r",
        this->currentTime(),
        (this->running()) ? "true" : "false",
        (this->isPrepTime()) ? "true" : "false",
        (this->roundInfo().isTraining()) ? "true" : "false",
        this->roundInfo().currentRound());
}
