#ifndef _BEEPER_H_
#define _BEEPER_H_

#include <stdint.h>
#include <lib/peripheral/gpioex.h>

class Beeper
{
    private:
        GPIOPin m_beeperPin;

    public:
        Beeper(uint32_t beeperPin);
        
        void beep(uint8_t beepCount);

};

#endif