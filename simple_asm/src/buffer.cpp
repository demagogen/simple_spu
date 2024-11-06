#include <assert.h>
#include "asm_typedefs.h"
#include "buffer.h"

BUFFER_ERROR buffer_init(BUFFER* bufferInfo)
{
    assert(bufferInfo && "null pointer on bufferInfo in asm_init_buffer\n");

    bufferInfo->argue   = 0;
    bufferInfo->ip      = 0;
    bufferInfo->reg     = 0;
    bufferInfo->buffer  = NULL;

    return BUFFER_NONE;;
}

BUFFER_ERROR buffer_set_size(BUFFER* bufferInfo, size_t size)
{
    assert(bufferInfo && "null pointer on bufferInfo in buffer_set_size\n");

    bufferInfo->buffer = (char* ) calloc(1, size);
    bufferInfo->size   = size;

    return BUFFER_NONE;
}

BUFFER_ERROR buffer_dump(BUFFER* bufferInfo)
{
    printf("in buffer_dump\n");
    assert(bufferInfo && "null pointer on bufferInfo in buffer_dump\n");

    // FILE* log_file = fopen("buffer_log_file.log", "wb");
    FILE* log_file = stdout;
    if (!log_file)
    {
        printf("error error error in log_file opening\n");
    }

    fprintf(log_file, "Buffer dump\n\n");
    fprintf(log_file, "Buffer size: %d\n",  bufferInfo->ip);
    fprintf(log_file, "Buffer argue: %d\n", bufferInfo->argue);
    fprintf(log_file, "Buffer reg: %d\n",   bufferInfo->reg);
    fprintf(log_file, "Buffer size: %d\n",  bufferInfo->size);

    fprintf(log_file, "Print buffer\n");
    for (size_t buffer_element = 0; buffer_element < bufferInfo->size; buffer_element++)
    {
        fprintf(log_file, "\tbuffer[%3d] = %8x", buffer_element, bufferInfo->buffer[buffer_element]);
        fprintf(log_file, "\t%8b\n", bufferInfo->buffer[buffer_element]);
    }

    fprintf(log_file, "\n\nend buffer dump\n");

    fclose(log_file);

    return BUFFER_NONE;
}
