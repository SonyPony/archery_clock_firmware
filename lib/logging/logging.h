#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdint.h>

#define PC_UART_CHANNEL 1

void uart_printf(const char *format, ...);

#endif