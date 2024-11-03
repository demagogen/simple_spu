#ifndef SPU_COMMANDS_H_
#define SPU_COMMANDS_H_

#include "stack_typedefs.h"

SPU_ERROR   spu_run_program       (SPU* spuInfo);
SPU_ERROR   spu_init_program_code (SPU* spuInfo);
SPU_ERROR   spu_dtor_stack        (SPU* spuInfo);
SPU_ERROR   spu_init_commands     (SPU* spuInfo);
SPU_ERROR   spu_push              (SPU* spuInfo, StackElem_t *push_element);
SPU_ERROR   spu_pop               (SPU* spuInfo, StackElem_t *pop_element);
SPU_ERROR   spu_add               (SPU* spuInfo);
SPU_ERROR   spu_mult              (SPU* spuInfo);
SPU_ERROR   spu_div               (SPU* spuInfo);
SPU_ERROR   spu_sub               (SPU* spuInfo);
SPU_ERROR   spu_jumps_parse       (SPU* spuInfo);
SPU_ERROR   spu_in                (SPU* spuInfo);
SPU_ERROR   spu_sqrt              (SPU* spuInfo);
SPU_ERROR   spu_syntax_error      (SPU* spuInfo);
SPU_ERROR   spu_out               (SPU* spuInfo);
SPU_ERROR   spu_call              (SPU* spuInfo);
SPU_ERROR   spu_ret               (SPU* spuInfo);
StackElem_t spu_get_arg           (SPU* spuInfo);

#endif
