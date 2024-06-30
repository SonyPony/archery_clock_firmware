#include "logging.h"
#include <stdarg.h>
#include <stdio.h>
//#include "lib/io/uart.h"
#include <string.h>
#include <pico/stdio.h>

//char print_buffer[512];
void Logging::log(const char *format, ...)
{
    //memset(print_buffer, 0, 512);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    //uart_send(PC_UART_CHANNEL, print_buffer);
}