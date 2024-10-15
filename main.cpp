#include "stack.h"
#include "spu_utils.h"
#include "spu_commands.h"

int main(int argc, const char* argv[]) {
    PROGRAM_CODE programCodeInfo = {};
    programCodeInfo.stackInfo = {};

    spu_init_program_code_info (&programCodeInfo, argc, argv);
    spu_init_program_code      (&programCodeInfo);
    spu_run_program            (&programCodeInfo);

    return 0;
}
