#ifndef _GPIOEX_H_
#define _GPIOEX_H_

#include <lib/core/port_linked_pin.h>

void GPIOex_Init(PortLinkedPin *pin, GPIO_Mode_TypeDef mode);
void GPIOex_Write(PortLinkedPin *pin, bool value);
void GPIOex_WriteHigh(PortLinkedPin *pin);
void GPIOex_WriteLow(PortLinkedPin *pin);
void GPIOex_WriteReverse(PortLinkedPin *pin);

#endif