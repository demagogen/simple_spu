#include <assert.h>
#include <string.h>
#include "text_data.h"
#include "asm.h"

ASM_ERROR asm_read_file(PROGRAM_CODE* programCodeInfo, const int argc, const char* argv[])
{
    asm_init_program_code_struct ( programCodeInfo);
    asm_open_input_file          ( programCodeInfo, argc, argv);
    count_symbols                ( programCodeInfo->input_file, &programCodeInfo->text_data);
    fill_text                    ( programCodeInfo->input_file, &programCodeInfo->text_data);
    buffer_set_size              (&programCodeInfo->bufferInfo, programCodeInfo->text_data.lines * (sizeof(int) + 2 * sizeof(char)));
    asm_fill_buffer              ( programCodeInfo);
    asm_create_output_file       ( programCodeInfo);

    asm_labels_dump(programCodeInfo);

    return ASM_NONE;
}

ASM_ERROR asm_fill_buffer(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_fill_buffer\n");
    for (size_t line_pointer_index = 0; line_pointer_index < programCodeInfo->text_data.lines; line_pointer_index++)
    {
        char command[30] = {};
        int  offset      =  0;
        sscanf(programCodeInfo->text_data.LineData[line_pointer_index].lines_pointers, " %s%n", command, &offset); // FIXME to much cringe
        programCodeInfo->line_ip = line_pointer_index;
        asm_parse_commands(programCodeInfo, command, offset);
        programCodeInfo->line_ip++;
        programCodeInfo->bufferInfo.ip++;
    }

    return ASM_NONE;
}

ASM_ERROR asm_remove_comments(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_remove_comments\n");

    for (size_t symbol_index = 0; symbol_index < programCodeInfo->text_data.digits; symbol_index++)
    {
        if (programCodeInfo->text_data.text[symbol_index] == ';')
        {
            programCodeInfo->text_data.text[symbol_index] = '\0';
        }
    }

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

// 0x31 0x01 0x00 0x00 0x00

// struct Foo {
    // char opcode : 4;
    // char isReg : 1;
    // char isImm : 1;
// };

ASM_ERROR asm_create_output_file(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_create_output_file\n");

    programCodeInfo->output_file = fopen("a.bin", "w");
    if (!programCodeInfo->output_file)
    {
        printf("null pointer on output_file in asm_create_output_file\n");
        return ASM_OUTPUT_FILE_ALLOCATION_ERROR;
    }

    buffer_dump(&programCodeInfo->bufferInfo);
    fwrite(programCodeInfo->bufferInfo.buffer, sizeof(char), programCodeInfo->bufferInfo.size, programCodeInfo->output_file);

    fclose(programCodeInfo->output_file);

    return ASM_NONE;
}

ASM_ERROR asm_init_program_code_struct(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_init_program_code_struct\n");

    programCodeInfo->input_file   = NULL;
    programCodeInfo->output_file  = NULL;
    programCodeInfo->program_code = NULL;
    // programCodeInfo->labels       = {};
    programCodeInfo->label_ip     = 0;
    programCodeInfo->size         = 0;
    programCodeInfo->line_ip      = 0;

    for (size_t label_index = 0; label_index < programCodeInfo->label_ip; label_index++)
    {
        programCodeInfo->labels[label_index].label_name    = NULL;
        programCodeInfo->labels[label_index].label_pointer = POISON_LABEL;
    }

    buffer_init(&programCodeInfo->bufferInfo);

    return ASM_NONE;
}

ASM_ERROR asm_parse_commands(PROGRAM_CODE* programCodeInfo, char* command, int offset)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_parse_commands\n");

    if (strcmp(command, "hlt" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= HLT;

        return ASM_NONE;
    }
    if (strcmp(command, "out" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= OUT;

        return ASM_NONE;
    }
    if (strcmp(command, "push") == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= PUSH;
        asm_push_or_pop_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "pop" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= POP;
        asm_push_or_pop_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "add" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= ADD;

        return ASM_NONE;
    }
    if (strcmp(command, "sub" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= SUB;

        return ASM_NONE;
    }
    if (strcmp(command, "mult") == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= MULT;

        return ASM_NONE;
    }
    if (strcmp(command, "div" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= DIV;

        return ASM_NONE;
    }
    if (strcmp(command, "in"  ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= IN;

        return ASM_NONE;
    }
    if (strcmp(command, "jmp" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= JMP;
        asm_jumps_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "ja"  ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= JA;
        asm_jumps_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "jae" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= JAE;
        asm_jumps_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "jb"  ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= JB;
        asm_jumps_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "jbe" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= JBE;
        asm_jumps_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "je"  ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= JE;
        asm_jumps_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "jne" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= JNE;
        asm_jumps_parse_arguments(programCodeInfo, offset);

        return ASM_NONE;
    }
    if (strcmp(command, "call") == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= CALL;
        asm_jumps_parse_arguments(programCodeInfo, offset);
    }
    if (strcmp(command, "ret" ) == 0)
    {
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= RET;
    }
    if (strchr(programCodeInfo->text_data.LineData[programCodeInfo->line_ip].lines_pointers, ':'))
    {
        // FIXME somewhere pointer is on word in my local variable
        // FIXME somewhere pointer is on the line on my
        // FIXME programCodeInfo->text_data.LineData[programCodeInfo->line_ip].lines_pointers
        // FIXME like here
        // FIXME It's cringe =_=
        command = programCodeInfo->text_data.LineData[programCodeInfo->line_ip].lines_pointers;
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] |= ADD_LABEL;
        asm_label_parse(programCodeInfo, command, offset);

        return ASM_NONE;
    }

    return ASM_INVALID_COMMAND;
}

// ______________________________________________PARSE FUNCTIONS ARGUMENTS___________________________________________________

ASM_ERROR asm_push_or_pop_parse_arguments(PROGRAM_CODE* programCodeInfo, int offset)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_push_parse_arguments\n");

    int   value            = 0;
    char  register_id      = 0;
    int   offset_increment = 0;
    char  opcode  = programCodeInfo->bufferInfo.buffer [programCodeInfo->bufferInfo.ip];
    char* command = programCodeInfo->text_data.LineData[programCodeInfo->line_ip].lines_pointers;

    if (sscanf(command + offset, " [ r%cx%n", &register_id, &offset_increment) == 1)
    {
        offset += offset_increment;
        if (strchr(command + offset, '+'))
        {
            opcode |= REG | RAM | IMM;
            sscanf(command + offset, " + %d ]", &value);
        }
        else
        {
            opcode |= RAM | REG;
            sscanf(command + offset, " [r%cx]", &register_id);
        }
    }
    else if (sscanf(command + offset, " [ %d ]", &value) == 1)
    {
        printf("ram immediate command: %s\n", command);
        opcode |= IMM | RAM;
    }
    else if (sscanf(command + offset, " r%cx", &register_id) == 1)
    {
        printf("register command: %s\n", command);
        opcode |= REG;
    }
    else if (sscanf(command + offset, " %d", &value) == 1)
    {
        printf("immediate command: %s\n", command);
        opcode |= IMM;
    }

    programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] = opcode;

    if (opcode & REG)
    {
        programCodeInfo->bufferInfo.ip++;
        asm_register_stupid_parse(&register_id);
        programCodeInfo->bufferInfo.buffer[programCodeInfo->bufferInfo.ip] = register_id;
    }
    if (opcode & IMM)
    {
        programCodeInfo->bufferInfo.ip++;
        *(int* )(programCodeInfo->bufferInfo.buffer + programCodeInfo->bufferInfo.ip) = value;
        programCodeInfo->bufferInfo.ip += sizeof(int) - 1;
    }

    return ASM_NONE;
}

ASM_ERROR asm_jumps_parse_arguments(PROGRAM_CODE* programCodeInfo, int offset)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_jumps_parse_arguments\n");

    programCodeInfo->bufferInfo.ip++;
    char* command = programCodeInfo->text_data.LineData[programCodeInfo->line_ip].lines_pointers;
    char jump_point[30] = {};
    sscanf(command + offset, "%s", jump_point);

    for (size_t label_index = 0; label_index < labels_quantity_const; label_index++)
    {
        if (strcmp(programCodeInfo->labels[label_index].label_name, jump_point) == 0)
        {
            *(int* )(programCodeInfo->bufferInfo.buffer + programCodeInfo->bufferInfo.ip) =
                programCodeInfo->labels[label_index].label_pointer;
            programCodeInfo->bufferInfo.ip += sizeof(int) - 1;

            return ASM_NONE;
        }
    }
    for (size_t label_index = 0; label_index < labels_quantity_const; label_index++)
    {
        if (programCodeInfo->labels[label_index].label_name = NULL)
        {
            programCodeInfo->labels[label_index].label_name    = jump_point;
            programCodeInfo->labels[label_index].label_pointer = POISON_LABEL;

            return ASM_NONE;
        }
    }

    return ASM_INVALID_LABEL;
}

ASM_ERROR asm_label_parse(PROGRAM_CODE* programCodeInfo, char* command, int offset)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_label_parse\n");

    programCodeInfo->labels[programCodeInfo->label_ip].label_name    = command;
    programCodeInfo->labels[programCodeInfo->label_ip].label_pointer = programCodeInfo->bufferInfo.ip + 1;
    programCodeInfo->label_ip++;
    asm_labels_dump(programCodeInfo);
    buffer_dump(&programCodeInfo->bufferInfo);

    return ASM_NONE;
}

ASM_ERROR asm_register_stupid_parse(char* register_id)
{
    assert(register_id && "null pointer on register_id in asm_register_stupid_parse\n");

    if (*register_id == 'a') *register_id = 1;
    if (*register_id == 'b') *register_id = 2;
    if (*register_id == 'c') *register_id = 3;
    if (*register_id == 'd') *register_id = 4;

    return ASM_NONE;
}

ASM_ERROR asm_labels_dump(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo);

    FILE* log_file = fopen("labels_dump.log", "wb");

    fprintf(log_file, "Labels dump\n\n");

    for (size_t label_index = 0; label_index < labels_quantity_const; label_index++)
    {
        fprintf(log_file, "\tlabels[%2d]: %s = %d\n", label_index,
                                                      programCodeInfo->labels[label_index].label_name,
                                                      programCodeInfo->labels[label_index].label_pointer);
    }

    fprintf(log_file, "\nEnd labels dump\n");
    fclose(log_file);

    return ASM_NONE;
}

ASM_ERROR asm_parse_callbacks(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo && "null pointer on programCodeInfo in asm_parse_callbacks\n");



    return ASM_NONE;
}
