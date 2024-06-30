#include "logging.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdio.h>

void Logging::log(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}