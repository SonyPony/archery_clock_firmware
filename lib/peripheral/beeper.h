#ifndef _BEEPER_H_
#define _BEEPER_H_

#include <stdint.h>
#include <lib/peripheral/gpioex.h>

class Beeber
{
    private:
        GPIOPin m_beeperPin;

    public:
        Beeber(uint32_t beeperPin);
        
        void beep(uint8_t beepCount);

};

#endif