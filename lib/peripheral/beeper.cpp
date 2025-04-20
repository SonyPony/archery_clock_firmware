#include "beeper.h"
#include "pico/stdlib.h"
#include <lib/logging/logging.h>

Beeper::Beeper(uint32_t beeperPin): m_beeperPWM(beeperPin), m_currentBeepCounter(0)
{}

void Beeper::beep(uint8_t beepCount) {
    this->m_currentBeepCounter = beepCount;
    
    this->singleBeep();     // it will automatically follow up with other beeps
}

void Beeper::singleBeep() {
    if(this->m_currentBeepCounter <= 0) 
        return;

    Logging::log(LoggingLevel::Debug, "Start beep\n");

    this->m_beeperPWM.setParameters(this->m_signalFrequency, 50);
    this->m_beeperPWM.start();
    this->m_currentBeepCounter--;

    // add callback to stop beep and possibly start pause between beeps
    add_alarm_in_ms(        
        this->m_beepDuration, 
        Beeper::handleSingleBeepEnd,
        this, 
        false
    );
}

int64_t Beeper::handleSingleBeepEnd(alarm_id_t alaramId, void* data) {
    Beeper* self = static_cast<Beeper*>(data);
    self->m_beeperPWM.stop();
    Logging::log(LoggingLevel::Debug, "End beep.\n");

    if(self->m_currentBeepCounter <= 0)   // no more beeps, end it
        return false;

    // start pause between beeps
    add_alarm_in_ms(
        self->m_betweenBeepsPauseDuration,
        Beeper::handleSingleBeepStart,
        self,
        false
    );

    return false;
}

int64_t Beeper::handleSingleBeepStart(alarm_id_t alaramId, void* data) {
    Beeper* self = static_cast<Beeper*>(data);

    if(self->m_currentBeepCounter <= 0)   // no more beeps, end it
        return false;

    self->singleBeep();

    return false;
}