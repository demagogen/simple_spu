#include <assert.h>
#include <string.h>
#include "asm.h"

ASM_ERROR asm_read_file(PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[])
{
    asm_open_input_file      (programCodeInfo, argc, argv);
    asm_count_symbols_in_file(programCodeInfo);
    asm_read_commands_in_file(programCodeInfo);
    asm_create_output_file   (programCodeInfo);

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
    if (!programCodeInfo->program_code) {
        // TODO add clever verification
        return ASM_PROGRAM_CODE_ALLOCATION_ERROR;
    }

    fread(programCodeInfo->program_code, sizeof(char), programCodeInfo->size, programCodeInfo->input_file);
    asm_remove_newline_symbols(programCodeInfo);
    asm_count_words_in_file   (programCodeInfo);

    return ASM_NONE;
}

ASM_ERROR asm_command_switch(PROGRAM_CODE* programCodeInfo, const char* command)
{
    if (!programCodeInfo)
    {
        return ASM_STRUCT_ALLOCATION_ERROR;
    }
    if (strcmp(command, "hlt"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", HLT ); return ASM_NONE;}
    if (strcmp(command, "out"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", OUT ); return ASM_NONE;}
    if (strcmp(command, "push" ) == 0) {fprintf(programCodeInfo->output_file, "%d ", PUSH); return ASM_NONE;}
    if (strcmp(command, "pop"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", POP ); return ASM_NONE;}
    if (strcmp(command, "add"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", ADD ); return ASM_NONE;}
    if (strcmp(command, "sub"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", SUB ); return ASM_NONE;}
    if (strcmp(command, "mult" ) == 0) {fprintf(programCodeInfo->output_file, "%d ", MULT); return ASM_NONE;}
    if (strcmp(command, "div"  ) == 0) {fprintf(programCodeInfo->output_file, "%d ", DIV ); return ASM_NONE;}
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
    for (ssize_t string_index = 0; string_index < programCodeInfo->words_quantity; string_index++)
    {
        asm_command_switch(programCodeInfo, programCodeInfo->word_pointer[string_index]);
    }
    fclose(programCodeInfo->output_file);

    return ASM_NONE;
}
