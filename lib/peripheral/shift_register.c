#include "shift_register.h"
#include <stdlib.h>
#include <gpioex.h>
#include <delay.h>

void sr_init(ShiftRegister *sr, PortLinkedPin *clk_pin, PortLinkedPin *data_pin, PortLinkedPin *cs_pin)
{
    if (sr == NULL || clk_pin == NULL || data_pin == NULL || cs_pin == NULL)
        return;

    sr->clk_pin = clk_pin;
    sr->cs_pin = cs_pin;
    sr->data_pin = data_pin;

    GPIOex_Init(clk_pin, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIOex_Init(data_pin, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIOex_Init(cs_pin, GPIO_MODE_OUT_PP_HIGH_SLOW);
}

void sr_set_data(ShiftRegister *sr, uint8_t data)
{
    if (sr == NULL)
        return;

    uint8_t bit_mask;
    uint8_t i = 0;

    for (bit_mask = 0b10000000; bit_mask; bit_mask >>= 1)
    {
        GPIOex_Write(sr->data_pin, bit_mask & data); // sent bit to the SR
        for (i = 0; i < 2; i++)                         // send clock pulse
        {
            GPIOex_WriteReverse(sr->clk_pin); // the SR will react on rising edge of clock
            _delay_us(1);
        }
    }
}

void sr_confirm(ShiftRegister *sr)
{
    if (sr == NULL)
        return;

    GPIOex_WriteHigh(sr->cs_pin);
    _delay_us(1);
    GPIOex_WriteLow(sr->cs_pin);
}