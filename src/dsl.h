#ifndef DSL_H
#define DSL_H

#define _GET_ARG(num)                                                                 \
    get_arg(code_array.p + cpu_storage.registers[PC] + num * ARG_SIZE, &cpu_storage)

#define _HLT                                            \
{                                                       \
    asm_code_destructor(&code_array);                   \
    cpu_destructor(&cpu_storage);                       \
    return;                                             \
}

#define _TOP                                            \
    StackTop(&cpu_storage.stk)                                  

#define _PUSH(arg)                                      \
{                                                       \
    StackPush(&cpu_storage.stk, (arg));                 \
}

#define _POP                                            \
    StackPop(&cpu_storage.stk)

#define _JMP(arg)                                       \
{                                                       \
    cpu_storage.registers[PC] = (arg) - 1 - ARG_SIZE;   \
}

#endif