#ifndef SPU_COMMANDS_H_
#define SPU_COMMANDS_H_

#include "stack_typedefs.h"

SPU_ERROR spu_run_program       (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_init_program_code (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_dtor_stack        (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_init_commands     (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_commands_push     (PROGRAM_CODE* programCodeInfo, StackElem_t *push_element);
SPU_ERROR spu_commands_pop      (PROGRAM_CODE* programCodeInfo, StackElem_t *pop_element);
SPU_ERROR spu_commands_add      (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_commands_mult     (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_commands_div      (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_commands_sub      (PROGRAM_CODE* programCodeInfo);
SPU_ERROR spu_commands_hlt      (PROGRAM_CODE* programCodeInfo);

#endif
