#include "spu_typedefs.h"
#include "stack.h"
#include "spu_utils.h"
#include "spu_commands.h"
#include <assert.h>

const ssize_t start_stack_const = 4;

SPU_ERROR spu_run_program(SPU* spuInfo)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    for (spuInfo->instructional_pointer; spuInfo->instructional_pointer < spuInfo->size;
         spuInfo->instructional_pointer++)
    {
        char tmp = 0;
        tmp = spu_init_commands(spuInfo);
        if (tmp == SPU_END_PROGRAM)
        {
            break;
        }
    }

    stack_dump(&spuInfo->stackInfo);

    return SPU_NONE;
}

SPU_ERROR spu_init_program_code(SPU* spuInfo)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    spuInfo->instructional_pointer = 0;
    stack_ctor(&spuInfo->stackInfo, start_stack_const);

    return SPU_NONE;
}

SPU_ERROR spu_dtor_stack(SPU* spuInfo)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }
    stack_dtor      (&spuInfo->stackInfo);
    spu_close_files (spuInfo);

    return SPU_NONE;
}

SPU_ERROR spu_init_commands(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->program_code[spuInfo->instructional_pointer] == _hlt)
    {
        return SPU_END_PROGRAM;
    }
    if (spuInfo->program_code[spuInfo->instructional_pointer] == _push)
    {
        spuInfo->instructional_pointer++;
        spu_push(spuInfo, &spuInfo->program_code[spuInfo->instructional_pointer]);

        return SPU_NONE;
    }
    if (spuInfo->program_code[spuInfo->instructional_pointer] == _pop)
    {
        StackElem_t tmp = 0;
        spu_pop(spuInfo, &tmp);

        return SPU_NONE;
    }
    if (spuInfo->program_code[spuInfo->instructional_pointer] == _add)
    {
        spu_add(spuInfo);

        return SPU_NONE;
    }
    if (spuInfo->program_code[spuInfo->instructional_pointer] == _sub)
    {
        spu_sub(spuInfo);

        return SPU_NONE;
    }
    if (spuInfo->program_code[spuInfo->instructional_pointer] == _mult)
    {
        spu_mult(spuInfo);

        return SPU_NONE;
    }
    if (spuInfo->program_code[spuInfo->instructional_pointer] == _div)
    {
        spu_div(spuInfo);

        return SPU_NONE;
    }
    return SPU_NONE;
}

SPU_ERROR spu_push(SPU* spuInfo, StackElem_t* push_element)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    stack_push(&spuInfo->stackInfo, *push_element);

    return SPU_NONE;
}

SPU_ERROR spu_pop(SPU* spuInfo, StackElem_t* pop_element)
{
    assert(spuInfo);

    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    stack_pop(&spuInfo->stackInfo, pop_element);

    return SPU_NONE;
}

SPU_ERROR spu_add(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_ADD;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_pop  (spuInfo, &var1);
    spu_pop  (spuInfo, &var2);
    StackElem_t result = var1 + var2;
    spu_push (spuInfo, &result);

    return SPU_NONE;
}
// #define PROCESSOR_COMMAND(name, ...) spu_commands_##name(PROGRAM_CODE *spuInfo) {
//checks
//  __VA_ARGS__
// return SPU_NONE;
// }

// PROCESSOR_COMMAND(sub,
// StackElem_t var1 = pop();
// StackElem_t var2 = pop();
// push(var1 - var2)
//)

SPU_ERROR spu_sub(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_SUB;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_pop  (spuInfo, &var1);
    spu_pop  (spuInfo, &var2);
    StackElem_t result = var2 - var1;
    spu_push (spuInfo, &result);

    return SPU_NONE;
}

SPU_ERROR spu_mult(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_MULT;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_pop  (spuInfo, &var1);
    spu_pop  (spuInfo, &var2);
    StackElem_t result = var1 * var2;
    spu_push (spuInfo, &result);

    return SPU_NONE;
}

SPU_ERROR spu_div(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_DIV;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_pop  (spuInfo, &var1);
    spu_pop  (spuInfo, &var2);
    StackElem_t result = var2 / var1;
    if (var1 == 0)
    {
        return SPU_INVALID_OPERATION_DIV_ON_ZERO;
    }
    spu_push (spuInfo, &result);

    return SPU_NONE;
}
