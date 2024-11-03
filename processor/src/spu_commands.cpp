#include "spu_typedefs.h"
#include "stack.h"
#include "spu_utils.h"
#include "spu_commands.h"
#include "spu_define_utils.h"
#include "spu_debug.h"
#include <assert.h>

SPU_ERROR spu_run_program(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    for (; spuInfo->instructional_pointer < spuInfo->size;
         spuInfo->instructional_pointer++)
    {
        // printf("%d: command: %b\n", spuInfo->instructional_pointer, spuInfo->program_code[spuInfo->instructional_pointer]);
        char tmp = 0;
        tmp = spu_init_commands(spuInfo);
        // stack_dump(&spuInfo->stackInfo);
        // for (size_t ram_element = 0; ram_element < 5; ram_element++)
        // {
            // printf("%d: ram element = %d\n", ram_element, spuInfo->ram[ram_element]);
        // }
        // stack_dump(&spuInfo->stackInfo);

        // stack_dump(&spuInfo->stackInfo);
        if (tmp == SPU_END_PROGRAM)
        {
            break;
        }
    }

    return SPU_NONE;
}

SPU_ERROR spu_init_program_code(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    spuInfo->instructional_pointer = 0;
    spuInfo->stackInfo             = {};
    stack_ctor(&spuInfo->stackInfo, StartStackConst);

    return SPU_NONE;
}

SPU_ERROR spu_dtor_stack(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

    char command = spuInfo->program_code[spuInfo->instructional_pointer];

    static int init_calls = 0;
    init_calls++;

    if (command == 0)
    {
        return SPU_NONE;
    }
    else if (command == HLT)
    {
        return SPU_END_PROGRAM;
    }
    else if (ADD_LABEL == command)
    {
        return SPU_NONE;
    }
    else if (command == CALL)
    {
        spu_call(spuInfo);

        return SPU_NONE;
    }
    else if (command == OUT)
    {
        spu_out(spuInfo);

        return SPU_NONE;
    }
    else if (command == RET)
    {
        spu_ret(spuInfo);

        return SPU_NONE;
    }
    else if (command == ADD)
    {
        spu_add(spuInfo);

        return SPU_NONE;
    }
    else if (command == SUB)
    {
        spu_sub(spuInfo);

        return SPU_NONE;
    }
    else if (command == MULT)
    {
        spu_mult(spuInfo);

        return SPU_NONE;
    }
    else if (command == DIV)
    {
        spu_div(spuInfo);

        return SPU_NONE;
    }
    else if (command == IN)
    {
        spu_in(spuInfo);

        return SPU_NONE;
    }
    else if ((command == JMP) ||
             (command == JA ) ||
             (command == JAE) ||
             (command == JB ) ||
             (command == JBE) ||
             (command == JE ) ||
             (command == JNE))
    {
        spu_jumps_parse(spuInfo);

        return SPU_NONE;
    }
    else if (command == SQRT)
    {
        spu_sqrt(spuInfo);

        return SPU_NONE;
    }
    else if ((command & PUSH) == PUSH)
    {
        StackElem_t tmp = spu_get_arg(spuInfo);
        spu_push(spuInfo, &tmp);
        // stack_dump(&spuInfo->stackInfo);

        return SPU_NONE;
    }
    else if ((command & POP) == POP) //TODO remove ULTIMATE CRINGE
    {
        StackElem_t tmp = 0;
        spu_pop(spuInfo, &tmp);
        char command = spuInfo->program_code[spuInfo->instructional_pointer];

        if (command & REG)
        {
            spuInfo->instructional_pointer++;
            char register_number = spuInfo->program_code[spuInfo->instructional_pointer];

            if (command & IMM)
            {
                spuInfo->instructional_pointer++;
                StackElem_t immediate_value = *(StackElem_t* )(spuInfo->program_code + spuInfo->instructional_pointer);

                if (command & RAM)
                {
                    StackElem_t register_value = spuInfo->registers_array[register_number];
                    spuInfo->ram[register_value + immediate_value] = tmp;
                    spu_dump(spuInfo);
                }
                else
                {
                    spu_syntax_error(spuInfo);
                }
            }
            if (command & RAM)
            {
                spuInfo->ram[spuInfo->registers_array[register_number]] = tmp;
            }
            else
            {
                spuInfo->registers_array[register_number] = tmp;
            }

            return SPU_NONE;
        }
        else
        {
            if (command & IMM)
            {
                spuInfo->instructional_pointer++;
                StackElem_t immediate_value = *(StackElem_t* )(spuInfo->program_code + spuInfo->instructional_pointer);

                if (command & RAM)
                {
                    spuInfo->ram[immediate_value] = tmp;
                    spu_dump(spuInfo);
                    spuInfo->instructional_pointer += sizeof(int) - 1;
                }
                else
                {
                    spu_syntax_error(spuInfo);
                }
            }
        }
        // stack_dump(&spuInfo->stackInfo);

        return SPU_NONE;
    }
    else
    {
        spu_syntax_error(spuInfo);

        return SPU_SYNTAX_ERROR;
    }

    return SPU_NONE;
}

SPU_ERROR spu_push(SPU* spuInfo, StackElem_t* push_element)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (spuInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_ADD;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_pop (spuInfo, &var1);
    spu_pop (spuInfo, &var2);
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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (spuInfo->stackInfo.size < 1)
    {
        return SPU_NOT_SUITABLE_SIZE_FOR_DIV;
    }

    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_pop (spuInfo, &var1);
    spu_pop (spuInfo, &var2);
    if (var1 == 0)
    {
        return SPU_INVALID_OPERATION_DIV_ON_ZERO;
    }
    StackElem_t result = var2 / var1;
    // printf("spu_div result = %d\n", result);
    // exit(0);
    spu_push (spuInfo, &result);

    return SPU_NONE;
}

SPU_ERROR spu_call(SPU* spuInfo)
{
    assert(spuInfo && "null pointer on spuInfo in spu_call\n");

    for (size_t return_func_index = 0; return_func_index < ReturnFunctionsPointersQuantityConst; return_func_index++)
    {
        if (spuInfo->return_pointer[return_func_index] == -1) //TODO POISON return
        {
            spuInfo->return_pointer_index++;
            spuInfo->return_pointer[return_func_index] = spuInfo->instructional_pointer + sizeof(int);
            spuInfo->instructional_pointer++;
            spuInfo->instructional_pointer = *(int* )(spuInfo->program_code + spuInfo->instructional_pointer) - 1;
            // break;
            return SPU_NONE;
        }
    }

    // for (size_t return_func_index = 0; return_func_index < ReturnFunctionsPointersQuantityConst;
        //  return_func_index++)
    // {
        // printf("%d: %d\n", return_func_index, spuInfo->return_pointer[return_func_index]);
    // }
    // printf("________________________\n");
    // printf("________________________\n");
    // printf("________________________\n");
    // static int pages = 0;
    // pages++;
    // if (pages == 10)
    // {
        // exit(0);
    // }

    return SPU_RETURN_POINTER_ALLOCATION_ERROR;
}

SPU_ERROR spu_ret(SPU* spuInfo)
{
    assert(spuInfo && "null pointer on spuInfo in spu_ret\n");

    spuInfo->instructional_pointer = spuInfo->return_pointer[spuInfo->return_pointer_index];
    spuInfo->return_pointer[spuInfo->return_pointer_index] = -1;
    spuInfo->return_pointer_index--;

    // static int pencil = 0;
    // pencil++;
    // if (pencil == 2)
    // {
        // exit(0);
    // }

    return SPU_NONE;
}

SPU_ERROR spu_jumps_parse(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

    //TODO remove cringe leveling (3rd string in define
#define INIT_POINTER                                                                   \
    spuInfo->instructional_pointer++;                                                  \
    int jump_point = *(int* )(spuInfo->program_code + spuInfo->instructional_pointer) - 1;\
    spuInfo->instructional_pointer = jump_point;                                       \

    int val1 = 0;
    int val2 = 0;
    spu_pop(spuInfo, &val1);
    spu_pop(spuInfo, &val2);

    if (spuInfo->program_code[spuInfo->instructional_pointer] == JA)
    {
        if (val1 < val2)
        {
            INIT_POINTER;
        }
        else
        {
            spuInfo->instructional_pointer += sizeof(int);
        }
    }
    else if (spuInfo->program_code[spuInfo->instructional_pointer] == JAE)
    {
        if (val1 <= val2)
        {
            INIT_POINTER;
        }
        else
        {
            spuInfo->instructional_pointer += sizeof(int);
        }
    }
    else if (spuInfo->program_code[spuInfo->instructional_pointer] == JB)
    {
        if (val1 > val2)
        {
            INIT_POINTER;
        }
        else
        {
            spuInfo->instructional_pointer += sizeof(int);
        }
    }
    else if (spuInfo->program_code[spuInfo->instructional_pointer] == JBE)
    {
        if (val1 >= val2)
        {
            INIT_POINTER;
        }
        else
        {
            spuInfo->instructional_pointer += sizeof(int);
        }
    }
    else if (spuInfo->program_code[spuInfo->instructional_pointer] == JE)
    {
        if (val1 == val2)
        {
            INIT_POINTER;
        }
        else
        {
            spuInfo->instructional_pointer += sizeof(int);
        }
    }
    else if (spuInfo->program_code[spuInfo->instructional_pointer] == JNE)
    {
        if (val1 != val2)
        {
            INIT_POINTER;
        }
        else
        {
            spuInfo->instructional_pointer += sizeof(int);
        }
    }
    else if (spuInfo->program_code[spuInfo->instructional_pointer] == JMP)
    {
        INIT_POINTER;
    }
    else
    {
        spu_syntax_error(spuInfo);
    }

    spu_push(spuInfo, &val2);
    spu_push(spuInfo, &val1);

#undef INIT_POINTER

    return SPU_NONE;
}

SPU_ERROR spu_in(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    StackElem_t entered_value = 0;
    printf("Enter value: _\b");
    scanf("%d", &entered_value);
    spu_push(spuInfo, &entered_value);

    return SPU_NONE;
}

SPU_ERROR spu_sqrt(SPU* spuInfo)
{
    assert(spuInfo && "null pointer on spuInfo in spu_sqrt\n");

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

    StackElem_t val = 0;
    spu_pop(spuInfo, &val);
    val = sqrt(val);
    spu_push(spuInfo, &val);

    return SPU_NONE;
}

SPU_ERROR spu_syntax_error(SPU* spuInfo)
{
    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    spuInfo->error = SPU_SYNTAX_ERROR;
    spu_dump(spuInfo);

    return SPU_SYNTAX_ERROR;
}

SPU_ERROR spu_out(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    StackElem_t out_element = 0;
    spu_pop(spuInfo, &out_element);
    printf("%d\n", out_element);

    return SPU_NONE;
}

StackElem_t spu_get_arg(SPU* spuInfo)
{
    // TODO megadefine
    assert(spuInfo && "null pointer on spuInfo in spu_getarg\n");

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

    char        reg_number      = POISON_REGISTER;
    StackElem_t immediate_value = 0;
    char command = spuInfo->program_code[spuInfo->instructional_pointer];

    if (command & RAM)
    {
        if (command & REG)
        {
            spuInfo->instructional_pointer++;
            reg_number = spuInfo->program_code[spuInfo->instructional_pointer];
        }
        if (command & IMM)
        {
            spuInfo->instructional_pointer++;
            immediate_value = *(StackElem_t* )((char* )spuInfo->program_code + spuInfo->instructional_pointer);
            spuInfo->instructional_pointer += sizeof(int) - 1;
        }

        if (command & REG && command & IMM)
        {
            return spuInfo->ram[reg_number + immediate_value];
        }
        else if (!(command & REG) && command & IMM)
        {
            return spuInfo->ram[immediate_value];
        }
        else if (command & REG && !(command & IMM))
        {
            return spuInfo->ram[reg_number];
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (command & REG)
        {
            spuInfo->instructional_pointer++;
            reg_number = spuInfo->program_code[spuInfo->instructional_pointer];
        }
        if (command & IMM)
        {
            spuInfo->instructional_pointer++;
            immediate_value = *(StackElem_t* )(spuInfo->program_code + spuInfo->instructional_pointer);
            spuInfo->instructional_pointer += sizeof(int) - 1;
        }
        if ((command & REG) && (command & IMM))
        {
            return SPU_INVALID_ARGUE;
        }
        else if (!(command & REG) && (command & IMM))
        {
            return immediate_value;
        }
        else if (command & REG && !(command & IMM))
        {
            return spuInfo->registers_array[reg_number];
        }
        else
        {
            return 0;
        }
    }

    return -1;
}

SPU_ERROR spu_get_register_value(SPU* spuInfo, char* register_number, char* register_value)
{
    assert(spuInfo && "null pointer on spuInfo in spu_get_register_value\n");

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

    if (*register_number == POISON_REGISTER)
    {
        return SPU_INVALID_REGISTER;
    }
    else
    {
        *register_value = spuInfo->registers_array[*register_number];
    }

    return SPU_NONE;
}
