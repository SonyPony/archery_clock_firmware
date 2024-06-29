#ifndef _GPIOEX_H_
#define _GPIOEX_H_

#include <pico/stdlib.h>


enum GPIOMode {
    GPIOIn = GPIO_IN,
    GPIOOut = GPIO_OUT
};

void GPIOex_Init(uint32_t pin, enum GPIOMode mode);
void GPIOex_Write(uint32_t pin, bool value);
void GPIOex_WriteHigh(uint32_t pin);
void GPIOex_WriteLow(uint32_t pin);

#endif