#ifndef _PIN_MAPPING_
#define _PIN_MAPPING_

#include <stm8s_gpio.h>

typedef struct
{
    GPIO_TypeDef* port;
    GPIO_Pin_TypeDef pin;
} PortLinkedPin;    // pin linked with port

#endif