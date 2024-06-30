#ifndef _GPIOEX_H_
#define _GPIOEX_H_

#include <pico/stdlib.h>


enum GPIOMode {
    GPIOIn = GPIO_IN,
    GPIOOut = GPIO_OUT
};

class GPIOPin {
    protected:
        uint32_t m_pin;
    
    public:
        GPIOPin(uint32_t pin, enum GPIOMode mode, bool defaultValue = false);

        void write(bool value);
        void writeHigh();
        void writeLow();
};

#endif