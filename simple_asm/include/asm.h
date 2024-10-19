#ifndef ASM_H_
#define ASM_H_

#include <ctype.h>
#include <stdlib.h>
#include <cstdio>

const int labels_quantity_const = 30;

enum ASM_ERROR
{
    ASM_NONE                          = 0,
    ASM_INPUT_FILE_ALLOCATION_ERROR   = 1,
    ASM_OUTPUT_FILE_ALLOCATION_ERROR  = 2,
    ASM_PROGRAM_CODE_ALLOCATION_ERROR = 3,
    ASM_STRUCT_ALLOCATION_ERROR       = 4,
    ASM_INVALID_SIZE                  = 5,
    ASM_INVALID_COMMAND               = 6
};

enum REGISTER
{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3
};

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
    IN    = 10,
    JMP   = 11,
    JA    = 12,
    JAE   = 13,
    JB    = 14,
    JBE   = 15,
    JE    = 16,
    JNE   = 17
};

enum LABEL_ENUM
{
    POISON_LABEL = -1
};

struct LABEL
{
    char* label_name;
    int   label_pointer;
};

struct PROGRAM_CODE
{
    FILE*   input_file;
    FILE*   output_file;
    size_t  size;
    size_t  words_quantity;
    size_t  label_ip;
    LABEL   labels[labels_quantity_const];
    char**  word_pointer;
    char*   program_code;
};

ASM_ERROR asm_read_file                (PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[]);
ASM_ERROR asm_open_input_file          (PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[]);
ASM_ERROR asm_count_symbols_in_file    (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_count_words_in_file      (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_read_commands_in_file    (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_remove_newline_symbols   (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_read_commands_in_file    (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_command_switch           (PROGRAM_CODE* programCodeInfo, const char* command);
ASM_ERROR asm_create_output_file       (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_init_program_code_struct (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_set_jumps                (PROGRAM_CODE* programCodeInfo);

#endif
