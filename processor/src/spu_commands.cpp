#include "spu_typedefs.h"
#include "stack.h"
#include "spu_utils.h"
#include "spu_commands.h"
#include "spu_define_utils.h"
#include "spu_debug.h"
#include <assert.h>

const ssize_t start_stack_const = 4;

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

    for (spuInfo->instructional_pointer; spuInfo->instructional_pointer < spuInfo->size;
         spuInfo->instructional_pointer++)
    {
        printf("spuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        char tmp = 0;
        tmp = spu_init_commands(spuInfo);
        // stack_dump(&spuInfo->stackInfo);

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

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    if (!spuInfo)
    {
        return SPU_PROGRAM_CODE_STRUCT_ALLOCATION_ERROR;
    }

    spuInfo->instructional_pointer = 0;
    spuInfo->stackInfo = {};
    stack_ctor(&spuInfo->stackInfo, start_stack_const);

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

    if ((command & ADD_LABEL) == command)
    {
        printf("ADD_LABEL\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        return SPU_NONE;
    }
    else if ((command & HLT) == command)
    {
        printf("HLT\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        return SPU_END_PROGRAM;
    }
    else if (command == ADD)
    {
        printf("ADD\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        spu_add(spuInfo);

        return SPU_NONE;
    }
    else if (command == SUB)
    {
        printf("SUB\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        spu_sub(spuInfo);

        return SPU_NONE;
    }
    else if (command == MULT)
    {
        printf("MULT\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        spu_mult(spuInfo);

        return SPU_NONE;
    }
    else if (command == DIV)
    {
        printf("DIV\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        spu_div(spuInfo);

        return SPU_NONE;
    }
    else if (command == IN)
    {
        printf("IN\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        spu_in(spuInfo);

        return SPU_NONE;
    }
    else if (command == OUT)
    {
        printf("OUT\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        spu_out(spuInfo);

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
        printf("JUMPS\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        spu_jumps_parse(spuInfo);

        return SPU_NONE;
    }
    else if ((command & PUSH) == PUSH)
    {
        printf("PUSH\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
        StackElem_t tmp = spu_get_arg(spuInfo);
        spu_push(spuInfo, &tmp);
        // stack_dump(&spuInfo->stackInfo);

        return SPU_NONE;
    }
    else if ((command & POP) == POP)
    {
        printf("POP\tspuInfo->instructional_pointer = %5d\n", spuInfo->instructional_pointer);
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
                    printf("error error error\n");
                    printf("register_number = %d\n", register_number);
                    printf("immediate_number = %d\n", immediate_value);
                    StackElem_t register_value = spuInfo->registers_array[register_number];
                    spuInfo->ram[register_value + immediate_value] = tmp;
                    printf("\tvalue in ram array: %d\n",
                        spuInfo->ram[register_value + immediate_value]);
                    spu_dump(spuInfo);
                    exit(0);
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
                printf("\tvalue in registers_array: %d\n",
                    spuInfo->registers_array[register_number]);
            }
        }
        // stack_dump(&spuInfo->stackInfo);

        return SPU_NONE;
    }
    else
    {
        printf("\t\t\tsyntax error\n");
        printf("\t\t\tcommand: %b\n", spuInfo->program_code[spuInfo->instructional_pointer]);
        printf("JMP: %b\n", JMP);
        printf("JA : %b\n", JA);
        printf("JAE: %b\n", JAE);
        printf("JB : %b\n", JB);
        printf("JBE: %b\n", JBE);
        printf("JE : %b\n", JNE);
        printf("binary equation result: %b\n", spuInfo->program_code[spuInfo->instructional_pointer] & JA);
        if (spuInfo->program_code[spuInfo->instructional_pointer] == JA)
        {
            printf("valid\n");
        }
        printf("cmd: %b\n", spuInfo->program_code[spuInfo->instructional_pointer - 1]);
        printf("\t\t\tcommand: %b\n", spuInfo->program_code[spuInfo->instructional_pointer]);
        printf("error after printf\n");
        spu_syntax_error(spuInfo);
        printf("error in spu_syntax_error\n");

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

    printf("push_element = %d\n", *push_element);
    printf("spuInfo->stackInfo.size = %d\n", spuInfo->stackInfo.size);
    stack_push(&spuInfo->stackInfo, *push_element);
    printf("spuInfo->stackInfo.size = %d\n", spuInfo->stackInfo.size);

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

    printf("spuInfo->stackInfo.size = %d\n", spuInfo->stackInfo.size);
    stack_pop(&spuInfo->stackInfo, pop_element);
    printf("spuInfo->stackInfo.size = %d\n", spuInfo->stackInfo.size);

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
    stack_dump(&spuInfo->stackInfo);
    spu_pop (spuInfo, &var1);
    spu_pop (spuInfo, &var2);
    printf("var1 = %d\nvar2 = %d\n", var1, var2);
    StackElem_t result = var1 + var2;
    spu_push (spuInfo, &result);
    exit(0);

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

    printf("i am in the div\n");
    StackElem_t var1 = 0;
    StackElem_t var2 = 0;
    spu_pop (spuInfo, &var1);
    stack_dump(&spuInfo->stackInfo);
    printf("stackInfo->pointer = %d\n", spuInfo->stackInfo.size);
    spu_pop (spuInfo, &var2);
    printf("var1: %d\nvar2: %d\n", var1, var2);
    if (var1 == 0)
    {
        return SPU_INVALID_OPERATION_DIV_ON_ZERO;
    }
    StackElem_t result = var2 / var1;
    spu_push (spuInfo, &result);

    return SPU_NONE;
}

SPU_ERROR spu_jumps_parse(SPU* spuInfo)
{
    assert(spuInfo);

    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }

#define INIT_POINTER                                                                   \
    spuInfo->instructional_pointer++;                                                  \
    int jump_point = *(int* )(spuInfo->program_code + spuInfo->instructional_pointer); \
    spuInfo->instructional_pointer  = jump_point;                                      \

    int val1 = 0;
    int val2 = 0;
    spu_pop(spuInfo, &val1);
    spu_pop(spuInfo, &val2);

    printf("command: %b\n", spuInfo->program_code[spuInfo->instructional_pointer]);
    exit(0);

    if ((spuInfo->program_code[spuInfo->instructional_pointer] & JA) == JA)
    {
        printf("in JA\n");
        exit(0);
        if (val1 < val2)
        {
            INIT_POINTER;
            printf("jump point value: %b\n", spuInfo->program_code[spuInfo->instructional_pointer]);
            exit(0);
        }
        else
        {
            spuInfo->instructional_pointer += sizeof(int);
        }
    }
    else if ((spuInfo->program_code[spuInfo->instructional_pointer] & JAE) == JAE)
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
    else if ((spuInfo->program_code[spuInfo->instructional_pointer] & JB) == JB)
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
    else if ((spuInfo->program_code[spuInfo->instructional_pointer] & JBE) == JBE)
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
    else if ((spuInfo->program_code[spuInfo->instructional_pointer] & JE) == JE)
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
    else if ((spuInfo->program_code[spuInfo->instructional_pointer] & JNE) == JNE)
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
    else if ((spuInfo->program_code[spuInfo->instructional_pointer] & JMP) == JMP)
    {
        INIT_POINTER;
    }
    else
    {
        printf("ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR\n");
        printf("spuInfo->program_code[spuInfo->instructional_pointer] = %b\n",
            spuInfo->program_code[spuInfo->instructional_pointer]);
        printf("error error error in jumps_parsing\n");
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

SPU_ERROR spu_syntax_error(SPU* spuInfo)
{
    printf("\t\t\tin spu_syntax_error\n");
    if (spuInfo->error != SPU_NONE)
    {
        return spuInfo->error;
    }
    spuInfo->error = SPU_SYNTAX_ERROR;
    spu_dump(spuInfo);
    printf("command: %b\n", spuInfo->program_code[spuInfo->instructional_pointer]);
    exit(0);

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
            return spuInfo->ram[reg_number + immediate_value];
        }
        else if (command & REG && !(command & IMM))
        {
            return spuInfo->ram[immediate_value];
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
            printf("reg not ram command: %b\n", command);
            spuInfo->instructional_pointer++;
            reg_number = spuInfo->program_code[spuInfo->instructional_pointer];
        }
        if (command & IMM)
        {
            printf("command: %b\n", spuInfo->program_code[spuInfo->instructional_pointer]);
            spuInfo->instructional_pointer++;
            immediate_value = *(StackElem_t* )(spuInfo->program_code + spuInfo->instructional_pointer);
            spuInfo->instructional_pointer += sizeof(int) - 1;
        }
        if (command & REG && command & IMM)
        {
            return SPU_INVALID_ARGUE;
        }
        else if (!(command & REG) && (command & IMM))
        {
            return immediate_value;
        }
        else if (command & REG && !(command & IMM))
        {
            return spuInfo->ram[immediate_value];
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
