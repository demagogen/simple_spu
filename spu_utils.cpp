#include "spu_utils.h"

SPU_ERROR spu_init_program_code_info(PROGRAM_CODE* programCodeInfo, int argc, const char* argv[])
{
    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    spu_init_files             (programCodeInfo, argc, argv);
    spu_init_program_code_size (programCodeInfo);
    spu_read_program_code      (programCodeInfo);

    return SPU_NONE;
}

SPU_ERROR spu_dtor_program_code_info(PROGRAM_CODE* programCodeInfo)
{
    fclose(programCodeInfo->input_file);
    fclose(programCodeInfo->output_file);
    programCodeInfo->size = 0;
    programCodeInfo->program_code = 0;

    return SPU_NONE;
}

SPU_ERROR spu_init_files(PROGRAM_CODE* programCodeInfo, int argc, const char* argv[])
{
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
        programCodeInfo->output_file = stdout;
    }
    else if (argc == 3)
    {
        programCodeInfo->input_file  = fopen(argv[1], "rb");
        programCodeInfo->output_file = fopen(argv[2], "w");
    }

    if (!programCodeInfo->input_file)  return SPU_INPUT_FILE_ALLOCATION_ERROR;
    if (!programCodeInfo->output_file) return SPU_OUTPUT_FILE_ALLOCATION_ERROR;

    return SPU_NONE;
}

SPU_ERROR spu_init_program_code_size(PROGRAM_CODE* programCodeInfo)
{
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

    if (programCodeInfo->size < 0) return SPU_INVALID_SIZE;

    return SPU_NONE;
}

SPU_ERROR spu_read_program_code(PROGRAM_CODE* programCodeInfo)
{
    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fseek(programCodeInfo->input_file, 0, SEEK_SET);
    programCodeInfo->program_code = (int* ) calloc(programCodeInfo->size, sizeof(int));
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

SPU_ERROR spu_close_files(PROGRAM_CODE* programCodeInfo)
{
    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    fclose(programCodeInfo->input_file);
    fclose(programCodeInfo->output_file);

    return SPU_NONE;
}
