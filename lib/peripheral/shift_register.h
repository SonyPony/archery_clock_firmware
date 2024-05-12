#ifndef _SHIFT_REGISTER_H_
#define _SHIFT_REGISTER_H_

#include <port_linked_pin.h>
#include <stdint.h>

typedef struct
{
    PortLinkedPin *clk_pin;
    PortLinkedPin *data_pin;
    PortLinkedPin *cs_pin;
} ShiftRegister;

void sr_init(ShiftRegister *sr, PortLinkedPin *clk_pin, PortLinkedPin *data_pin, PortLinkedPin *cs_pin);

/**
 * @brief sr_set_Data Set data to a shift register. However, the data is not set to the output, it needs to be confirmed.
 */
void sr_set_data(ShiftRegister *sr, uint8_t data);

/**
 * @brief sr_confirm Data that has been sent to the shift register will be set to output pins of the register after confirmation.
 */
void sr_confirm(ShiftRegister *sr);

#endif