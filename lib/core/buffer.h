#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

typedef struct
{
    volatile uint8_t *data;
    volatile int* data_end_idx;
    int size;
} Buffer;

/**
 * @brief buffer_clear Sets the buffer data entries to zero.
 * @param buffer Buffer that will be cleaned.
 */
void buffer_clear(Buffer *buffer);

#endif // BUFFER_H
