#ifndef ASM_TYPEDEFS_H_
#define ASM_TYPEDEFS_H_

#include <ctype.h>
#include <stdlib.h>
#include <cstdio>

const int LabelsQuantityConst    = 30;
const int FunctionsQuantityConst = 30;

const int ImmediateOffset = sizeof(int) - 1;
const int PointerOffset   = sizeof(int) - 1;

enum ASM_ERROR
{
    ASM_NONE                          = 0,
    ASM_INPUT_FILE_ALLOCATION_ERROR   = 1,
    ASM_OUTPUT_FILE_ALLOCATION_ERROR  = 2,
    ASM_PROGRAM_CODE_ALLOCATION_ERROR = 3,
    ASM_STRUCT_ALLOCATION_ERROR       = 4,
    ASM_INVALID_SIZE                  = 5,
    ASM_INVALID_COMMAND               = 6,
    ASM_BUFFER_ALLOCATION_ERROR       = 7,
    ASM_INVALID_REGISTER_NUMBER       = 8,
    ASM_INVALID_LABEL                 = 9
};

enum REGISTER
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};

enum CMD_INFO
{
    IMM = 0b00000001,
    REG = 0b00000010,
    RAM = 0b00000100
};

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
    CALL      = 0b00010010,
    RET       = 0b00010011,
    SQRT      = 0b00000111
};

enum LABEL_ENUM
{
    POISON_LABEL = -1
};

#endif
