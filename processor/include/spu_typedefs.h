#ifndef SPU_TYPEDEFS_H_
#define SPU_TYPEDEFS_H_

#include "stack_typedefs.h"
#include <stdlib.h>
#include <stdio.h>

const int registers_quantity_const = 5;
const int labels_quantity_const    = 30;
const int ram_size_const           = 30;

enum REGISTER
{
    POISON_REGISTER = -1,
    NULL_REGISTER   =  0,
    AX              =  1,
    BX              =  2,
    CX              =  3,
    DX              =  4
};

enum CMD_INFO
{
    IMM = 0b00000001,
    REG = 0b00000010,
    RAM = 0b00000100
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
    SPU_INVALID_REGISTER                     = 15,
    SPU_INVALID_ARGUE                        = 16
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
    ERROR     = 0b00000000,
    HLT       = 0b00001000,
    OUT       = 0b00010000,
    PUSH      = 0b00011000,
    POP       = 0b00100000,
    ADD       = 0b00101000,
    SUB       = 0b00110000,
    MULT      = 0b00111000,
    DIV       = 0b01000000,
    IN        = 0b01001000,
    JMP       = 0b01010000,
    JA        = 0b01011000,
    JAE       = 0b01100000,
    JB        = 0b01101000,
    JBE       = 0b01110000,
    JE        = 0b01111000, // I use not formatter enum
    JNE       = 0b00001001,
    ADD_LABEL = 0b00010001,
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
    char*        program_code;
    StackElem_t  registers_array [registers_quantity_const];
    StackElem_t  ram             [ram_size_const];
    int          instructional_pointer;
    STACK        stackInfo;
};

#endif
