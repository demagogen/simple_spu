#include "spu_debug.h"

SPU_ERROR spu_dump_(SPU* spuInfo, const char* FILE__, const int LINE__, const char* func__)
{
    FILE* log_file = fopen("spu_debug.log", "w");
    if (!log_file)
    {
        printf("allocation error in spu_dump\n");
    }

    spu_dump_pretty_title(log_file);

    if (spuInfo->error == SPU_NONE)
    {
        fprintf(log_file, "no errors in spu\n");
    }
    else
    {
        fprintf(log_file, "%d\n", spuInfo->error);
        fprintf(log_file, "error in spu\n");
    }

    fprintf(log_file, "%s[%p] at %s:%d born at %s\n\n",      func__, func__, FILE__, LINE__, func__);
    fprintf(log_file, "input_file pointer: %p\n",            spuInfo->input_file);
    fprintf(log_file, "size of input_file: %lld\n",          spuInfo->size);
    fprintf(log_file, "instructional_pointer value: %lld\n", spuInfo->instructional_pointer);

    fprintf(log_file, "program code commands list: [\n");
    for (ssize_t program_code_command = 0; program_code_command < spuInfo->size; program_code_command++)
    {
        fprintf(log_file, "%llb\n", spuInfo->program_code[program_code_command]);
    }
    fprintf(log_file, "]\n\n");

    fprintf(log_file, "register array list:\n");
    for (size_t register_index = 0; register_index < RegistersQuantityConst; register_index++)
    {
        fprintf(log_file, "\tregister_array[%d] = %d\n", register_index, spuInfo->registers_array[register_index]);
    }

    fprintf(log_file, "RAM dump\n");
    for (size_t ram_index = 0; ram_index < RamSizeConst; ram_index++)
    {
        fprintf(log_file, "RAM[%3d] = %5d\n", ram_index, spuInfo->ram[ram_index]);
    }
    fprintf(log_file, "end RAM dump\n");

    fclose(log_file);

    return SPU_NONE;
}

SPU_ERROR spu_dump_pretty_title(FILE* log_file)
{
    fprintf(log_file, "                        _   \n"
"                       | |                        \n"
"  ___ _ __  _   _    __| |_   _ _ __ ___  _ __    \n"
" / __| '_ \\| | | |  / _` | | | | '_ ` _ \\| '_ \\\n"
" \\__ \\ |_) | |_| | | (_| | |_| | | | | | | |_) |\n"
" |___/ .__/ \\__,_|  \\__,_|\\__,_|_| |_| |_| .__/\n"
"     | |                                 | |      \n"
"     |_|                                 |_|      \n");

    return SPU_NONE;
}
