#include "spu_typedefs.h"
#include "stack.h"
#include "spu_utils.h"
#include "spu_commands.h"
#include <assert.h>

const ssize_t start_stack_const = 4;

SPU_ERROR spu_run_program(PROGRAM_CODE* programCodeInfo)
{
    assert(programCodeInfo);

    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    for (programCodeInfo->instructional_pointer; programCodeInfo->instructional_pointer < programCodeInfo->size;
         programCodeInfo->instructional_pointer++)
    {
        printf("\tcommand = %d\n", programCodeInfo->program_code[programCodeInfo->instructional_pointer]);
        char tmp = 0;
        tmp = spu_init_commands(programCodeInfo);
        if (tmp == SPU_END_PROGRAM)
        {
            break;
        }
    }

    stack_dump(&programCodeInfo->stackInfo);

    return SPU_NONE;
}

SPU_ERROR spu_init_program_code(PROGRAM_CODE* programCodeInfo)
{
    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    programCodeInfo->instructional_pointer = 0;
    stack_ctor(&programCodeInfo->stackInfo, start_stack_const);

    return SPU_NONE;
}

SPU_ERROR spu_dtor_stack(PROGRAM_CODE* programCodeInfo)
{
    printf("into spu_dtor_stack\n");
    if (!programCodeInfo)
    {
        printf("\t into assert\n");
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }
    printf("\t into spu_dtor_stack\n");
    stack_dtor      (&programCodeInfo->stackInfo);
    printf("\t passed stack_dtor\n");
    spu_close_files (programCodeInfo);
    printf("\t passed spu_close_files\n");

    return SPU_NONE;
}

SPU_ERROR spu_init_commands(PROGRAM_CODE* programCodeInfo)
{
    if (programCodeInfo->program_code[programCodeInfo->instructional_pointer] == _hlt)
    {
        return SPU_END_PROGRAM;
    }
    if (programCodeInfo->program_code[programCodeInfo->instructional_pointer] == _push)
    {
        programCodeInfo->instructional_pointer++;
        spu_commands_push(programCodeInfo, &programCodeInfo->program_code[programCodeInfo->instructional_pointer]);

        return SPU_NONE;
    }
    if (programCodeInfo->program_code[programCodeInfo->instructional_pointer] == _pop)
    {
        StackElem_t tmp = 0;
        spu_commands_pop(programCodeInfo, &tmp);

        return SPU_NONE;
    }
    if (programCodeInfo->program_code[programCodeInfo->instructional_pointer] == _add)
    {
        spu_commands_add(programCodeInfo);

        return SPU_NONE;
    }
    if (programCodeInfo->program_code[programCodeInfo->instructional_pointer] == _sub)
    {
        spu_commands_sub(programCodeInfo);

        return SPU_NONE;
    }
    if (programCodeInfo->program_code[programCodeInfo->instructional_pointer] == _mult)
    {
        spu_commands_mult(programCodeInfo);

        return SPU_NONE;
    }
    if (programCodeInfo->program_code[programCodeInfo->instructional_pointer] == _div)
    {
        spu_commands_div(programCodeInfo);

        return SPU_NONE;
    }
    return SPU_NONE;
}

SPU_ERROR spu_commands_push(PROGRAM_CODE* programCodeInfo, StackElem_t* push_element)
{
    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    stack_push(&programCodeInfo->stackInfo, *push_element);

    return SPU_NONE;
}

SPU_ERROR spu_commands_pop(PROGRAM_CODE* programCodeInfo, StackElem_t* pop_element)
{
    if (!programCodeInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    stack_pop(&programCodeInfo->stackInfo, pop_element);
    printf("popped value %d\n", pop_element);

    return SPU_NONE;
}

SPU_ERROR spu_commands_add(PROGRAM_CODE* programCodeInfo)
{
    if (programCodeInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_ADD;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_commands_pop  (programCodeInfo, &var1);
    printf("%d\n", var1);
    spu_commands_pop  (programCodeInfo, &var2);
    printf("%d\n", var2);
    printf("%d %d\n", var1, var2);
    StackElem_t result = var1 + var2;
    spu_commands_push (programCodeInfo, &result);

    return SPU_NONE;
}

SPU_ERROR spu_commands_sub(PROGRAM_CODE* programCodeInfo)
{
    if (programCodeInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_SUB;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_commands_pop  (programCodeInfo, &var1);
    spu_commands_pop  (programCodeInfo, &var2);
    StackElem_t result = var2 - var1;
    spu_commands_push (programCodeInfo, &result);

    return SPU_NONE;
}

SPU_ERROR spu_commands_mult(PROGRAM_CODE* programCodeInfo)
{
    if (programCodeInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_MULT;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_commands_pop  (programCodeInfo, &var1);
    spu_commands_pop  (programCodeInfo, &var2);
    StackElem_t result = var1 * var2;
    spu_commands_push (programCodeInfo, &result);

    return SPU_NONE;
}

SPU_ERROR spu_commands_div(PROGRAM_CODE* programCodeInfo)
{
    if (programCodeInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_DIV;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_commands_pop  (programCodeInfo, &var1);
    spu_commands_pop  (programCodeInfo, &var2);
    printf("passed spu_commands_pop\n");
    printf("%d %d\n", var1, var2);
    StackElem_t result = var2 / var1;
    if (var1 == 0)
    {
        return SPU_INVALID_OPERATION_DIV_ON_ZERO;
    }
    printf("took result\n");
    printf("%d\n", result);
    spu_commands_push (programCodeInfo, &result);
    printf("passed spu_commands_push\n");

    return SPU_NONE;
}
