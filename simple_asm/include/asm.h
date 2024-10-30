#ifndef ASM_H_
#define ASM_H_

#include <ctype.h>
#include <stdlib.h>
#include <cstdio>
#include "buffer.h"

struct LABEL
{
    char* label_name;
    int   label_pointer;
};

struct FUNCTIONS
{
    char* function_name;
    int   function_pointer;
};

struct PROGRAM_CODE
{
    FILE*     input_file;
    FILE*     output_file;
    size_t    size;
    size_t    line_ip;
    size_t    label_ip;
    LABEL     labels   [labels_quantity_const];
    FUNCTIONS functions[functions_quantity_const];
    TEXT_DATA text_data;
    BUFFER    bufferInfo;
    char*     program_code;
};

ASM_ERROR asm_read_file                   (PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[]);
ASM_ERROR asm_open_input_file             (PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[]);
ASM_ERROR asm_remove_comments             (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_create_output_file          (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_init_program_code_struct    (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_parse_commands              (PROGRAM_CODE* programCodeInfo, char* command, int offset);
ASM_ERROR asm_push_or_pop_parse_arguments (PROGRAM_CODE* programCodeInfo, int offset);
ASM_ERROR asm_jumps_parse_arguments       (PROGRAM_CODE* programCodeInfo, int offset);
ASM_ERROR asm_label_parse                 (PROGRAM_CODE* programCodeInfo, char* command, int offset);
ASM_ERROR asm_fill_buffer                 (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_register_stupid_parse       (char* register_id);
ASM_ERROR asm_labels_dump                 (PROGRAM_CODE* programCodeInfo);
ASM_ERROR asm_parse_callbacks             (PROGRAM_CODE* programCodeInfo);

#endif
