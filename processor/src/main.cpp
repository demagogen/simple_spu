#include "stack.h"
#include "spu_utils.h"
#include "spu_define_utils.h"
#include "spu_commands.h"
#include "spu_debug.h"

int main(int argc, const char* argv[])
{
    SPU spuInfo = {};
    spuInfo.stackInfo = {};

    spu_init_info         (&spuInfo, argc, argv);
    spu_init_program_code (&spuInfo);
    spu_run_program       (&spuInfo);
    spu_dump              (&spuInfo);
    spu_dtor_info         (&spuInfo);
    if (spuInfo.error != SPU_NONE)
    {
        spu_error_print(&spuInfo);
    }

    return 0;
}
