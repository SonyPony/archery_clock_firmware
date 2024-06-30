#include "shift_register.h"
#include <stdlib.h>
#include <lib/peripheral/gpioex.h>
#include <pico/stdlib.h>

ShiftRegister::ShiftRegister(uint32_t clkPin, uint32_t dataPin, uint32_t csPin) : m_clkPin(clkPin, GPIOMode::GPIOOut, false),
                                                                                  m_dataPin(dataPin, GPIOMode::GPIOOut, false),
                                                                                  m_csPin(csPin, GPIOMode::GPIOOut, false)
{
}

void ShiftRegister::setData(uint8_t data)
{
    for (uint8_t bit_mask = 0b10000000; bit_mask; bit_mask >>= 1)
    {
        this->m_dataPin.write(bit_mask & data); // sent bit to the SR

        this->m_clkPin.writeHigh(); // the SR will react on rising edge of clock
        sleep_us(1);
        this->m_clkPin.writeLow();
        sleep_us(1);
    }
}

void ShiftRegister::confirm()
{
    this->m_csPin.writeHigh();
    sleep_us(1);
    this->m_csPin.writeLow();
    sleep_us(1);
}