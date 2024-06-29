#include "gpioex.h"

void GPIOex_Init(uint32_t pin, enum GPIOMode mode)
{
    gpio_init(pin);
    gpio_set_dir(pin, (uint32_t)mode);
}

void GPIOex_Write(uint32_t pin, bool value)
{
    gpio_put(pin, value);
}

void GPIOex_WriteHigh(uint32_t pin)
{
     GPIOex_Write(pin, true);
}

void GPIOex_WriteLow(uint32_t pin)
{
    GPIOex_Write(pin, false);
}