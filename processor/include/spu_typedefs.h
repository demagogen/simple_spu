#ifndef SPU_TYPEDEFS_H_
#define SPU_TYPEDEFS_H_

#include "stack_typedefs.h"
#include <stdlib.h>
#include <stdio.h>

const int registers_quantity_const = 4;
const int labels_quantity_const   = 30;

enum REGISTER
{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3
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
    SPU_INVALID_OPERATION_DIV_ON_ZERO        = 13,
    SPU_INVALID_JUMP_POINTER                 = 14,
    SPU_INVALID_REGISTER                     = 15
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
    ERROR = -1,
    HLT   =  0,
    OUT   =  1,
    PUSH  =  2,
    POP   =  3,
    ADD   =  4,
    SUB   =  5,
    MULT  =  6,
    DIV   =  7,
    PUSHR =  8,
    POPR  =  9,
    IN    = 10
};

struct LABEL
{
    char* label_name;
    int   label_pointer;
};

struct SPU
{
    SPU_ERROR    error;
    FILE*        input_file;
    ssize_t      size;
    int*         program_code;
    StackElem_t  registers_array [registers_quantity_const];
    LABEL        labels[30];
    int          instructional_pointer;
    STACK        stackInfo;
};

#endif
