#ifndef DISASM_TYPEDEFS_H_
#define DISASM_TYPEDEFS_H_

#include "stdlib.h"

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

enum DISASM_ERROR
{
    DISASM_NONE,
    DISASM_INPUT_FILE_ALLOCATION_ERROR,
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
    JE        = 0b01111000,
    JNE       = 0b00001001,
    ADD_LABEL = 0b00010001,
    CALL      = 0b00010010,
    RET       = 0b00010011
};

struct LABEL
{
    char* label_name;
    int   label_pointer;
};

struct DISASM
{
    size_t size;
    char*  binary_code;
    size_t binary_code_ip;
    char*  buffer;
};

#endif
