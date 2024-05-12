#include "buffer.h"
#include <stdlib.h>

void buffer_clear(Buffer *buffer)
{
    if (buffer == NULL)
        return;

    if (buffer->data == NULL)
        return;

    for (int i = 0; i < buffer->size; i++)
        buffer->data[i] = 0;
}
