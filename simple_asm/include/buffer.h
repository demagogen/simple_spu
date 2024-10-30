#ifndef BUFFER_H_
#define BUFFER_H_

#include "asm_typedefs.h"
#include "text_data.h"

enum BUFFER_ERROR
{
    BUFFER_NONE             = 0,
    BUFFER_ALLOCATION_ERROR = 1,
    BUFFER_BAD_SIZE         = 2,
    BUFFER_BAD_COMMAND      = 3,
    BUFFER_BAD_IP           = 4,
    BUFFER_BAD_ARGUE        = 5,
    BUFFER_BAD_REG          = 6
};

struct BUFFER
{
    size_t    ip;
    int       argue;
    char      reg;
    size_t    size;
    char*     buffer;
};

BUFFER_ERROR buffer_init     (BUFFER* bufferInfo);
BUFFER_ERROR buffer_set_size (BUFFER* bufferInfo, size_t size);
BUFFER_ERROR buffer_dump     (BUFFER* bufferInfo);

#endif
