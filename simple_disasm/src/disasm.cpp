#include "disasm.h"

DISASM_ERROR disasm_init_struct(DISASM* disasmInfo)
{
    assert(disasmInfo);

    disasmInfo->size           = 0;
    disasmInfo->binary_code    = NULL;
    disasmInfo->binary_code_ip = 0;
    disasmInfo->buffer         = 0;

    return DISASM_NONE;
}

const char* print_command_enum()
{
    #define DESCRIPTION_(_error) \
        case _error: return #_error

    switch(stackInfo->error_info)
    {
        DESCRIPTION_(STACK_NONE);
        DESCRIPTION_(STACK_BAD_PTR);
        DESCRIPTION_(STACK_BAD_SIZE);
        DESCRIPTION_(STACK_UNDERFLOW);
        DESCRIPTION_(STACK_OVERFLOW);
        DESCRIPTION_(STACK_ALLOCATION_ERROR);

        default:
            return "error print error";
    }

    #undef DESCRIPTION_
}
