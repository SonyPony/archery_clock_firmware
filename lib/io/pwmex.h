#ifndef _PWMEX_
#define _PWMEX_

#include <stdint.h>


class PWMPin {
    protected:
        uint32_t m_pin;
        uint32_t m_slice;
        uint32_t m_channel;
        uint32_t m_clockFrequency;
        bool m_running;
        bool m_inited;

    public:
        PWMPin(uint32_t pin);
        PWMPin(uint32_t pin, uint32_t frequency, uint8_t DCL);

        bool start();
        bool stop();
        void setParameters(uint32_t frequency, uint8_t DCL);

        bool running() const;
};

#endif