#ifndef _SHIFT_REGISTER_H_
#define _SHIFT_REGISTER_H_

#include <stdint.h>
#include <lib/peripheral/gpioex.h>

class ShiftRegister
{
    private:
        GPIOPin m_clkPin;
        GPIOPin m_dataPin;
        GPIOPin m_csPin;

    public:
        ShiftRegister(uint32_t clkPin, uint32_t dataPin, uint32_t csPin);

        /**
         * @brief sr_set_Data Set data to a shift register. However, the data is not set to the output, it needs to be confirmed.
         */
        void setData(uint8_t data);

        /**
         * @brief sr_confirm Data that has been sent to the shift register will be set to output pins of the register after confirmation.
         */
        // TODO update doc
        void confirm();
};

#endif