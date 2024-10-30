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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

    fclose(spuInfo->input_file);
    spuInfo->size         = 0;
    spuInfo->program_code = 0;

    return SPU_NONE;
}

SPU_ERROR spu_init_files(SPU* spuInfo, int argc, const char* argv[])
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (!spuInfo)
    {
        spuInfo->error = SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;

        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    if (argc < 2)
    {
        spuInfo->error = SPU_INPUT_FILE_ALLOCATION_ERROR;

        return SPU_INPUT_FILE_ALLOCATION_ERROR;
    }
    else if (argc >= 3)
    {
        spuInfo->error = SPU_INPUT_FILE_ALLOCATION_ERROR;

        return SPU_INPUT_FILE_ALLOCATION_ERROR;
    }
    else if (argc == 2)
    {
        spuInfo->input_file = fopen(argv[1], "rb");
    }

    if (!spuInfo->input_file)
    {
        spuInfo->error = SPU_INPUT_FILE_ALLOCATION_ERROR;

        return SPU_INPUT_FILE_ALLOCATION_ERROR; // TODO do not write one line ifs
    }

    return SPU_NONE;
}

SPU_ERROR spu_init_program_code_size(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fseek(spuInfo->input_file, 0, SEEK_END);
    spuInfo->size = ftell(spuInfo->input_file);
    fseek(spuInfo->input_file, 0, SEEK_SET);
    spuInfo->program_code = (char* ) calloc(spuInfo->size, sizeof(char));

    return SPU_NONE;
}

SPU_ERROR spu_read_program_code(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fread(spuInfo->program_code, sizeof(char), spuInfo->size, spuInfo->input_file);
    fclose(spuInfo->input_file);

    return SPU_NONE;
}

SPU_ERROR spu_close_files(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fclose(spuInfo->input_file);

    return SPU_NONE;
}

const char* spu_error_print(SPU* spuInfo)
{
    #define DESCRIPTION_(error_) \
        case error_: return #error_

    switch(spuInfo->error)
    {
        DESCRIPTION_(SPU_NONE                                );
        DESCRIPTION_(SPU_INPUT_FILE_ALLOCATION_ERROR         );
        DESCRIPTION_(SPU_OUTPUT_FILE_ALLOCATION_ERROR        );
        DESCRIPTION_(SPU_INVALID_SIZE                        );
        DESCRIPTION_(SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR);
        DESCRIPTION_(SPU_PROGRAM_CODE_ALLOCATION_ERROR       );
        DESCRIPTION_(SPU_INVALID_COMMAND                     );
        DESCRIPTION_(SPU_SYNTAX_ERROR                        );
        DESCRIPTION_(SPU_NOT_SUITABLE_SIZE_FOR_ADD           );
        DESCRIPTION_(SPU_NOT_SUITABLE_SIZE_FOR_SUB           );
        DESCRIPTION_(SPU_NOT_SUITABLE_SIZE_FOR_MULT          );
        DESCRIPTION_(SPU_NOT_SUITABLE_SIZE_FOR_DIV           );
        DESCRIPTION_(SPU_END_PROGRAM                         );
        DESCRIPTION_(SPU_INVALID_OPERATION_DIV_ON_ZERO       );
        DESCRIPTION_(SPU_INVALID_JUMP_POINTER                );
        DESCRIPTION_(SPU_INVALID_REGISTER                    );

        default:
            return "error print error";
    }

    #undef DESCRIPTION_
}
