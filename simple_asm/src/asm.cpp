#include <assert.h>
#include <string.h>
#include "asm.h"

ASM_ERROR asm_read_file(PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[])
{
    asm_init_program_code_struct (programCodeInfo);
    asm_open_input_file          (programCodeInfo, argc, argv);
    asm_count_symbols_in_file    (programCodeInfo);
    asm_read_commands_in_file    (programCodeInfo);
    asm_create_output_file       (programCodeInfo);

    return ASM_NONE;
}

ASM_ERROR asm_open_input_file(PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[])
{
    if (argc < 2)
    {
        return ASM_INPUT_FILE_ALLOCATION_ERROR;
    }
    programCodeInfo->input_file = fopen(argv[1], "rb");
    if (!programCodeInfo->input_file)
    {
        return ASM_INPUT_FILE_ALLOCATION_ERROR;
    }

    if (!programCodeInfo->input_file)
    {
        return ASM_INPUT_FILE_ALLOCATION_ERROR;
    }

    return ASM_NONE;
}

ASM_ERROR asm_count_symbols_in_file(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_count_symbols_in_file\n");

    fseek(programCodeInfo->input_file, 0, SEEK_END);
    programCodeInfo->size = ftell(programCodeInfo->input_file);
    fseek(programCodeInfo->input_file, 0, SEEK_SET);
    if (!programCodeInfo->input_file)
    {
        // TODO I WILL MAKE IT BRIGHT!!!
        return ASM_INPUT_FILE_ALLOCATION_ERROR;
    }
    if (programCodeInfo->size < 0)
    {
        // TODO add return in log file
        return ASM_INVALID_SIZE;
    }

    return ASM_NONE;
}

ASM_ERROR asm_remove_newline_symbols(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_remove_newline_symbols\n");

    programCodeInfo->words_quantity = 0;
    for (ssize_t symbol_index = 0; symbol_index < programCodeInfo->size; symbol_index++)
    {
        if (programCodeInfo->program_code[symbol_index] == '\n' ||
            programCodeInfo->program_code[symbol_index] == ' ')
        {
            programCodeInfo->words_quantity++;
            programCodeInfo->program_code[symbol_index] = '\0';
        }
    }

    return ASM_NONE;
}

ASM_ERROR asm_count_words_in_file(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_count_words_in_file\n");

    programCodeInfo->word_pointer = (char** ) calloc(programCodeInfo->words_quantity + 1, sizeof(char*));
    ssize_t word_pointer_index = 0;
    programCodeInfo->word_pointer[word_pointer_index++] = programCodeInfo->program_code;
    for (ssize_t symbol_index = 0; symbol_index < programCodeInfo->size - 1; symbol_index++)
    {
        if (programCodeInfo->program_code[symbol_index] == '\0')
        {
            programCodeInfo->word_pointer[word_pointer_index++] = &programCodeInfo->program_code[symbol_index + 1];
        }
    }

    return ASM_NONE;
}

ASM_ERROR asm_read_commands_in_file(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_read_commands_in_file\n");

    programCodeInfo->program_code = (char*) calloc(programCodeInfo->size + 1, sizeof(char));
    if (!programCodeInfo->program_code)
    {
        // TODO add clever verification
        return ASM_PROGRAM_CODE_ALLOCATION_ERROR;
    }

    fread(programCodeInfo->program_code, sizeof(char), programCodeInfo->size, programCodeInfo->input_file);
    asm_remove_newline_symbols (programCodeInfo);
    asm_count_words_in_file    (programCodeInfo);

    return ASM_NONE;
}

// 0x31 0x01 0x00 0x00 0x00

struct Foo {
    char opcode : 4;
    char isReg : 1;
    char isImm : 1;
};

ASM_ERROR asm_command_switch(PROGRAM_CODE* programCodeInfo, const char* command)
{
    Foo foo = {};
    foo.opcode = HLT;
    foo.isImm = true;

    if (!programCodeInfo)
    {
        return ASM_STRUCT_ALLOCATION_ERROR;
    }
    if (strcmp(command, "hlt"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", HLT   ); return ASM_NONE;}
    if (strcmp(command, "out"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", OUT   ); return ASM_NONE;}
    if (strcmp(command, "push"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", PUSH  ); return ASM_NONE;}
    if (strcmp(command, "pop"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", POP   ); return ASM_NONE;}
    if (strcmp(command, "add"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", ADD   ); return ASM_NONE;}
    if (strcmp(command, "sub"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", SUB   ); return ASM_NONE;}
    if (strcmp(command, "mult"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", MULT  ); return ASM_NONE;}
    if (strcmp(command, "div"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", DIV   ); return ASM_NONE;}
    if (strcmp(command, "pushr" ) == 0) {fprintf(programCodeInfo->output_file, "%d ", PUSHR ); return ASM_NONE;}
    if (strcmp(command, "popr"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", POPR  ); return ASM_NONE;}
    if (strcmp(command, "AX"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", AX    ); return ASM_NONE;}
    if (strcmp(command, "BX"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", BX    ); return ASM_NONE;}
    if (strcmp(command, "CX"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", CX    ); return ASM_NONE;}
    if (strcmp(command, "DX"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", DX    ); return ASM_NONE;}
    if (strcmp(command, "in"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", IN    ); return ASM_NONE;}
    if (strcmp(command, "jmp"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", JMP   ); return ASM_NONE;}
    if (strcmp(command, "ja"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", JA    ); return ASM_NONE;}
    if (strcmp(command, "jae"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", JAE   ); return ASM_NONE;}
    if (strcmp(command, "jb"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", JB    ); return ASM_NONE;}
    if (strcmp(command, "jbe"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", JBE   ); return ASM_NONE;}
    if (strcmp(command, "je"    ) == 0) {fprintf(programCodeInfo->output_file, "%d ", JE    ); return ASM_NONE;}
    if (strcmp(command, "jne"   ) == 0) {fprintf(programCodeInfo->output_file, "%d ", JNE   ); return ASM_NONE;}
    else
    {
        fprintf(programCodeInfo->output_file, "%s ", command);
    }

    return ASM_NONE;
}

ASM_ERROR asm_create_output_file(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_create_output_file\n");

    programCodeInfo->output_file = fopen("a.asm", "w");
    if (!programCodeInfo->output_file)
    {
        printf("null pointer on output_file in asm_create_output_file\n");
        return ASM_OUTPUT_FILE_ALLOCATION_ERROR;
    }
    for (ssize_t word_pointer_index = 0; word_pointer_index < programCodeInfo->words_quantity; word_pointer_index++)
    {
        asm_command_switch(programCodeInfo, programCodeInfo->word_pointer[word_pointer_index]);
    }
    fclose(programCodeInfo->output_file);

    return ASM_NONE;
}

ASM_ERROR asm_init_program_code_struct(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_init_program_code_struct\n");

    *programCodeInfo->labels        = {};
    programCodeInfo->input_file     = NULL;
    programCodeInfo->label_ip       = 0;
    programCodeInfo->output_file    = NULL;
    programCodeInfo->program_code   = NULL;
    programCodeInfo->size           = 0;
    programCodeInfo->word_pointer   = 0;
    programCodeInfo->words_quantity = 0;

    return ASM_NONE;
}

ASM_ERROR asm_set_jumps(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_set_jumps\n");

    for (size_t word_index = 0; word_index < programCodeInfo->words_quantity; word_index++)
    {
        if (strcmp(programCodeInfo->word_pointer[word_index], "jmp" ) == 0 ||
            strcmp(programCodeInfo->word_pointer[word_index], "ja"  ) == 0 ||
            strcmp(programCodeInfo->word_pointer[word_index], "jae" ) == 0 ||
            strcmp(programCodeInfo->word_pointer[word_index], "jb"  ) == 0 ||
            strcmp(programCodeInfo->word_pointer[word_index], "jbe" ) == 0 ||
            strcmp(programCodeInfo->word_pointer[word_index], "je"  ) == 0 ||
            strcmp(programCodeInfo->word_pointer[word_index], "jne" ) == 0)
        {
        }
    }

    return ASM_NONE;
}
