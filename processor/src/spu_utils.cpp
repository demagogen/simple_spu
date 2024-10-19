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

SPU_ERROR spu_init_info(SPU* spuInfo, int argc, const char* argv[])
{
    assert(spuInfo);

    // 00000001 | 00010000 = 00010001
    // ERROR_1 = 00000001 => 00010001 & 00000001 == true

    //Foo foo{};
    //foo.err1 = 1;
    //if (foo.err3) {}

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    spuInfo->error = SPU_NONE;
    spu_init_files             (spuInfo, argc, argv);
    spu_init_program_code_size (spuInfo);
    spu_read_program_code      (spuInfo);

    return SPU_NONE;
}

SPU_ERROR spu_dtor_info(SPU* spuInfo)
{
    assert(spuInfo);

    fclose(spuInfo->input_file);
    spuInfo->size         = 0;
    spuInfo->program_code = 0;

    return SPU_NONE;
}

SPU_ERROR spu_init_files(SPU* spuInfo, int argc, const char* argv[])
{
    assert(spuInfo);

    if (!spuInfo)
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
        spuInfo->input_file = fopen(argv[1], "rb");
    }
    else if (argc == 3)
    {
        spuInfo->input_file  = fopen(argv[1], "rb");
    }

    if (!spuInfo->input_file)  return SPU_INPUT_FILE_ALLOCATION_ERROR; // TODO do not write one line ifs

    return SPU_NONE;
}

SPU_ERROR spu_init_program_code_size(SPU* spuInfo)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fseek(spuInfo->input_file, 0, SEEK_SET);
    spuInfo->size = 0;
    while (true)
    {
        int tmp = 0;
        if (fscanf(spuInfo->input_file, "%d", &tmp) == 1)
        {
            spuInfo->size++;
        }

        if (feof(spuInfo->input_file))
        {
            break;
        }
    }

    if (spuInfo->size < 0) return SPU_INVALID_SIZE; // TODO one line if

    return SPU_NONE;
}

SPU_ERROR spu_read_program_code(SPU* spuInfo)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fseek(spuInfo->input_file, 0, SEEK_SET);
    spuInfo->program_code = (int* ) calloc(spuInfo->size, sizeof(int));

    // TODO check calloc result

    for (ssize_t code_element_index = 0; code_element_index < spuInfo->size; code_element_index++)
    {
        int tmp = 0;
        if (fscanf(spuInfo->input_file, "%d", &tmp) == 1)
        {
            spuInfo->program_code[code_element_index] = tmp;
        }
    }

    return SPU_NONE;

}

SPU_ERROR spu_close_files(SPU* spuInfo)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fclose(spuInfo->input_file);

    return SPU_NONE;
}
