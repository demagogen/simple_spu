#ifndef SPU_COMMANDS_H_
#define SPU_COMMANDS_H_

#include "stack_typedefs.h"

SPU_ERROR spu_run_program       (SPU* spuInfo);
SPU_ERROR spu_init_program_code (SPU* spuInfo);
SPU_ERROR spu_dtor_stack        (SPU* spuInfo);
SPU_ERROR spu_init_commands     (SPU* spuInfo);
SPU_ERROR spu_push              (SPU* spuInfo, StackElem_t *push_element);
SPU_ERROR spu_pop               (SPU* spuInfo, StackElem_t *pop_element);
SPU_ERROR spu_add               (SPU* spuInfo);
SPU_ERROR spu_mult              (SPU* spuInfo);
SPU_ERROR spu_div               (SPU* spuInfo);
SPU_ERROR spu_sub               (SPU* spuInfo);
SPU_ERROR spu_hlt               (SPU* spuInfo);
SPU_ERROR spu_jump              (SPU* spuInfo, int jump_point);
SPU_ERROR spu_pushr             (SPU* spuInfo);
SPU_ERROR spu_popr              (SPU* spuInfo);
SPU_ERROR spu_in                (SPU* spuInfo);
SPU_ERROR spu_syntax_error      (SPU* spuInfo);
SPU_ERROR spu_out               (SPU* spuInfo);

#endif
