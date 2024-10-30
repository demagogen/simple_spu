#include "stack.h"
#include "spu_utils.h"
#include "spu_define_utils.h"
#include "spu_commands.h"
#include "spu_debug.h"

int main(int argc, const char* argv[])
{
    SPU spuInfo = {};
    spuInfo.stackInfo = {};

    printf("start spu\n");
    spu_init_info         (&spuInfo, argc, argv);
    printf("end spu_init_info\n");
    spu_init_program_code (&spuInfo);
    printf("end spu_init_program_code\n");
    spu_run_program       (&spuInfo);
    printf("end spu_run_program\n");
    spu_dump              (&spuInfo);
    printf("end spu_dump\n");
    spu_dtor_info         (&spuInfo);
    printf("end spu_dtor_info\n");
    if (spuInfo.error != SPU_NONE)
    {
        printf("error detected: %s\n", spu_error_print(&spuInfo));
    }

    return 0;
}
