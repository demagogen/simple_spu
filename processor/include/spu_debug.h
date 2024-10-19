#ifndef SPU_DEBUG_H_
#define SPU_DEBUG_H_

#include "spu_typedefs.h"
#include "spu_commands.h"
#include "spu_utils.h"

SPU_ERROR spu_dump_             (SPU* spuInfo, const char* FILE__, const int LINE__, const char* func__);
SPU_ERROR spu_dump_pretty_title (FILE* log_file);

#endif
