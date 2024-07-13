#include "logging.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <pico/stdio.h>

void Logging::log(LoggingLevel logLevel, const char *format, ...)
{
    switch (logLevel)
    {
    case LoggingLevel::Debug:
        printf("DEBUG: ");
        break;

    case LoggingLevel::Warning:
        printf("WARNING: ");
        break;

    case LoggingLevel::Info:
        printf("INFO: ");
        break;

    case LoggingLevel::Error:
        printf("ERROR: ");
        break;

    default:
        break;
    }
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void Logging::log(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}