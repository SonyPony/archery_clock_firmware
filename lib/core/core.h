#ifndef CORE_H
#define CORE_H

/**
 * @brief parse_int Converts a specified substring into a number. The function does not
 * check invalid characters. Thus, it could cause incorrect return value.
 * @param str Start of the parsed substring.
 * @param string_size Specifies the size of the substring
 * (number of characters) that will be processed.
 * @return Returns converted number from given substring.
 */
int parse_int(const char *str, int string_size);

#endif // CORE_H
