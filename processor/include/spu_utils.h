#ifndef SPU_UTILS_H_
#define SPU_UTILS_H_

#include "spu_typedefs.h"

SPU_ERROR spu_init_info              (SPU* programCodeInfo, int argc, const char* argv[]);
SPU_ERROR spu_dtor_info              (SPU* programCodeInfo);
SPU_ERROR spu_init_files             (SPU* programCodeInfo, int argc, const char* argv[]);
SPU_ERROR spu_init_program_code_size (SPU* programCodeInfo);
SPU_ERROR spu_read_program_code_size (SPU* programCodeInfo);
SPU_ERROR spu_read_program_code      (SPU* programCodeInfo);
SPU_ERROR spu_close_files            (SPU* programCodeInfo);

#endif
