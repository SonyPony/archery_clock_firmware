#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <stdint.h>

enum LoggingLevel {
    Info,
    Debug,
    Warning,
    Error,
};

namespace Logging {
    void log(const char *format, ...);
    void log(LoggingLevel logLevel, const char *format, ...);
}

#endif