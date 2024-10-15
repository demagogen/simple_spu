#ifndef SPU_UTILS_H_
#define SPU_UTILS_H_

#include "spu_typedefs.h"

SPU_ERROR spu_init_program_code_info (PROGRAM_CODE* programCodeInfo, int argc, const char* argv[]);
SPU_ERROR spu_dtor_program_code_info (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_init_files             (PROGRAM_CODE* programCodeInfo, int argc, const char* argv[]);
SPU_ERROR spu_init_program_code_size (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_read_program_code_size (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_read_program_code      (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_close_files            (PROGRAM_CODE* programCodeInfo);

#endif
