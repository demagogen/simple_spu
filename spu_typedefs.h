#ifndef SPU_TYPEDEFS_H_
#define SPU_TYPEDEFS_H_

#include "stack_typedefs.h"
#include <stdlib.h>
#include <stdio.h>

struct SPU
{
    FILE*   input_file;
    ssize_t size;
    int*    program_code;
    int     instructional_pointer;
    STACK   stackInfo;
};

enum SPU_ERROR
{
    SPU_NONE                                 =  0,
    SPU_INPUT_FILE_ALLOCATION_ERROR          =  1,
    SPU_OUTPUT_FILE_ALLOCATION_ERROR         =  2,
    SPU_INVALID_SIZE                         =  3,
    SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR =  4,
    SPU_PROGRAM_CODE_ALLOCATION_ERROR        =  5,
    SPU_INVALID_COMMAND                      =  6,
    SPU_SYNTAX_ERROR                         =  7,
    SPU_NOT_SUITABLE_SIZE_FOR_ADD            =  8,
    SPU_NOT_SUITABLE_SIZE_FOR_SUB            =  9,
    SPU_NOT_SUITABLE_SIZE_FOR_MULT           = 10,
    SPU_NOT_SUITABLE_SIZE_FOR_DIV            = 11,
    SPU_END_PROGRAM                          = 12,
    SPU_INVALID_OPERATION_DIV_ON_ZERO        = 13
};

//INSTRUCTION(PUSH, 5, {
//    push(value);
//})
//
//INSTRUCTION(ADD, 13, {
//    StackElem_t val1 = pop();
//    StackElem_t val2 = pop();
//    push(val1 + val2);
//})

enum PROCESSOR_COMMANDS
{
    _error = -1,
    _hlt   =  0,
    _out   =  1,
    _push  =  2,
    _pop   =  3,
    _add   =  4,
    _sub   =  5,
    _mult  =  6,
    _div   =  7
};

#endif
