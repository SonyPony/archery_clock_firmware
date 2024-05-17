#include "core.h"

int parse_int(const char *str, int string_size)
{
    int result = 0;
    for (int i = string_size - 1, decimal = 1; i >= 0; i--, decimal *= 10)
        result += (str[i] - '0') * decimal;

    return result;
}
