#include <assert.h>
#include "spu_utils.h"

// TODO asserts

//struct Command {
//    char opcode  : 4;
//    char isReg   : 1;
//    char isImmed : 1;
//};
//
// push opcode 0b0010 = 0x02
// 0b00.0.1.0010
// ... 0x12 0x0000000000000005 ...

SPU_ERROR spu_init_info(SPU* programCodeInfo, int argc, const char* argv[])
{
    assert(programCodeInfo);

    // 00000001 | 00010000 = 00010001
    // ERROR_1 = 00000001 => 00010001 & 00000001 == true

    //Foo foo{};
    //foo.err1 = 1;
    //if (foo.err3) {}

    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    spu_init_files             (programCodeInfo, argc, argv);
    spu_init_program_code_size (programCodeInfo);
    spu_read_program_code      (programCodeInfo);

    return SPU_NONE;
}

SPU_ERROR spu_dtor_info(SPU* programCodeInfo)
{
    assert(programCodeInfo);

    fclose(programCodeInfo->input_file);
    programCodeInfo->size = 0;
    programCodeInfo->program_code = 0;

    return SPU_NONE;
}

SPU_ERROR spu_init_files(SPU* programCodeInfo, int argc, const char* argv[])
{
    assert(programCodeInfo);

    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    if (argc < 2){
        return SPU_INPUT_FILE_ALLOCATION_ERROR;
    }
    else if (argc > 3)
    {
        return SPU_INPUT_FILE_ALLOCATION_ERROR;
    }
    else if (argc == 2)
    {
        programCodeInfo->input_file  = fopen(argv[1], "rb");
    }
    else if (argc == 3)
    {
        programCodeInfo->input_file  = fopen(argv[1], "rb");
    }

    if (!programCodeInfo->input_file)  return SPU_INPUT_FILE_ALLOCATION_ERROR; // TODO do not write one line ifs

    return SPU_NONE;
}

SPU_ERROR spu_init_program_code_size(SPU* programCodeInfo)
{
    assert(programCodeInfo);

    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fseek(programCodeInfo->input_file, 0, SEEK_SET);
    programCodeInfo->size = 0;
    while (true)
    {
        int tmp = 0;
        if (fscanf(programCodeInfo->input_file, "%d", &tmp) == 1)
        {
            programCodeInfo->size++;
        }

        if (feof(programCodeInfo->input_file))
        {
            break;
        }
    }

    if (programCodeInfo->size < 0) return SPU_INVALID_SIZE; // TODO one line if

    return SPU_NONE;
}

SPU_ERROR spu_read_program_code(SPU* programCodeInfo)
{
    assert(programCodeInfo);

    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fseek(programCodeInfo->input_file, 0, SEEK_SET);
    programCodeInfo->program_code = (int* ) calloc(programCodeInfo->size, sizeof(int));

    // TODO check calloc result

    for (ssize_t code_element_index = 0; code_element_index < programCodeInfo->size; code_element_index++)
    {
        int tmp = 0;
        if (fscanf(programCodeInfo->input_file, "%d", &tmp) == 1)
        {
            programCodeInfo->program_code[code_element_index] = tmp;
        }
    }

    return SPU_NONE;

}

SPU_ERROR spu_close_files(SPU* programCodeInfo)
{
    assert(programCodeInfo);

    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fclose(programCodeInfo->input_file);

    return SPU_NONE;
}