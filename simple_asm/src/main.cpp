#include "asm.h"

int main(int argc, const char* argv[])
{
    PROGRAM_CODE programCodeInfo = {};

    if (argc == 1)
    {
        printf("incorrect input\n");
    }
    if (argc == 2)
    {
        // TODO magic numbers and idiotic parsing
        asm_read_file(&programCodeInfo, argc, argv);
    }

    return 0;
}
