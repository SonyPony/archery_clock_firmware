#include "shift_register.h"
#include <stdlib.h>
#include <lib/peripheral/gpioex.h>

void sr_init(ShiftRegister *sr, uint32_t clk_pin, uint32_t data_pin, uint32_t cs_pin)
{
    if (sr == NULL)
        return;

    sr->clk_pin = clk_pin;
    sr->cs_pin = cs_pin;
    sr->data_pin = data_pin;

    gpio_init(clk_pin);
    gpio_init(data_pin);
    gpio_init(cs_pin);

    gpio_set_dir(clk_pin, GPIO_OUT);
    gpio_set_dir(data_pin, GPIO_OUT);
    gpio_set_dir(cs_pin, GPIO_OUT);
}

void sr_set_data(ShiftRegister *sr, uint8_t data)
{
    if (sr == NULL)
        return;

    for (uint8_t bit_mask = 0b10000000; bit_mask; bit_mask >>= 1)
    {
        GPIOex_Write(sr->data_pin, bit_mask & data); // sent bit to the SR
        for (uint8_t i = 0; i < 2; i++)                         // send clock pulse
        {
            GPIOex_WriteReverse(sr->clk_pin); // the SR will react on rising edge of clock
            _delay_us(1);   // TODO delay
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