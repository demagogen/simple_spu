#include "stack.h"
#include "spu_utils.h"
#include "spu_commands.h"

int main(int argc, const char* argv[]) {
    SPU spuInfo = {};
    spuInfo.stackInfo = {};

    spu_init_info         (&spuInfo, argc, argv);
    spu_init_program_code (&spuInfo);
    spu_run_program       (&spuInfo);
    spu_dtor_info         (&spuInfo);

    return 0;
}
