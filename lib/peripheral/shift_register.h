#ifndef _SHIFT_REGISTER_H_
#define _SHIFT_REGISTER_H_

#include <stdint.h>

typedef struct
{
    uint32_t clk_pin;
    uint32_t data_pin;
    uint32_t cs_pin;
} ShiftRegister;

void sr_init(ShiftRegister *sr, uint32_t clk_pin, uint32_t data_pin, uint32_t cs_pin);

/**
 * @brief sr_set_Data Set data to a shift register. However, the data is not set to the output, it needs to be confirmed.
 */
void sr_set_data(ShiftRegister *sr, uint8_t data);

/**
 * @brief sr_confirm Data that has been sent to the shift register will be set to output pins of the register after confirmation.
 */
void sr_confirm(ShiftRegister *sr);

#endif