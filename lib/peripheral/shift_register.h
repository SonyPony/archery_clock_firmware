#ifndef _SHIFT_REGISTER_H_
#define _SHIFT_REGISTER_H_

#include <port_linked_pin.h>

typedef struct
{
    PortLinkedPin *clk_pin;
    PortLinkedPin *data_pin;
    PortLinkedPin *cs_pin;
} ShiftRegister;

void sr_init(ShiftRegister *sr, PortLinkedPin *clk_pin, PortLinkedPin *data_pin, PortLinkedPin *cs_pin);

#endif