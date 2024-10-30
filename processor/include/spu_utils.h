#ifndef SPU_UTILS_H_
#define SPU_UTILS_H_

#include "spu_typedefs.h"

SPU_ERROR   spu_init_info              (SPU* spuInfo, int argc, const char* argv[]);
SPU_ERROR   spu_dtor_info              (SPU* spuInfo);
SPU_ERROR   spu_init_files             (SPU* spuInfo, int argc, const char* argv[]);
SPU_ERROR   spu_init_program_code_size (SPU* spuInfo);
SPU_ERROR   spu_read_program_code_size (SPU* spuInfo);
SPU_ERROR   spu_read_program_code      (SPU* spuInfo);
SPU_ERROR   spu_close_files            (SPU* spuInfo);
const char* spu_error_print            (SPU* spuInfo);

#endif
