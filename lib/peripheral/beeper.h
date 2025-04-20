#ifndef _BEEPER_H_
#define _BEEPER_H_

#include <stdint.h>
#include <lib/io/gpioex.h>
#include <lib/io/pwmex.h>
#include <hardware/clocks.h>

class Beeper
{
    private:
        PWMPin m_beeperPWM;
        uint32_t m_signalFrequency;               // in Hz for PWM
        uint32_t m_beepDuration;                  // in ms
        uint32_t m_betweenBeepsPauseDuration;     // in ms
        uint8_t m_currentBeepCounter;

    protected:
        static int64_t handleSingleBeepEnd(alarm_id_t alarmId, void* data);
        static int64_t handleSingleBeepStart(alarm_id_t alarmId, void* data);

        void singleBeep();


    public:
        Beeper(uint32_t beeperPin, uint32_t sinalFrequency, uint32_t beepDuration, uint32_t beepsPauseDuration);
        
        void beep(uint8_t beepCount);
};

#endif