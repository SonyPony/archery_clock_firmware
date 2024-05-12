#include "gpioex.h"

void GPIOex_Init(PortLinkedPin *pin, GPIO_Mode_TypeDef mode)
{
    GPIO_Init(pin->port, (GPIO_Pin_TypeDef)pin->pin, mode);
}

void GPIOex_Write(PortLinkedPin *pin, bool value)
{
    if (value)
        GPIOex_WriteHigh(pin);
    else
        GPIOex_WriteLow(pin);
}

void GPIOex_WriteHigh(PortLinkedPin *pin)
{
    GPIO_WriteHigh(pin->port, (GPIO_Pin_TypeDef)pin->pin);
}

void GPIOex_WriteLow(PortLinkedPin *pin)
{
    GPIO_WriteLow(pin->port, (GPIO_Pin_TypeDef)pin->pin);
}

void GPIOex_WriteReverse(PortLinkedPin *pin) {
    GPIO_WriteReverse(pin->port, (GPIO_Pin_TypeDef)pin->pin);
}