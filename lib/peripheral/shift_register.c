#include "shift_register.h"
#include <stdlib.h>
#include <gpioex.h>

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