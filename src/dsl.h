#ifndef DSL_H
#define DSL_H

#define _GET_ARG(num)                                                                 \
	get_arg(code_array.p + cpu_storage.registers[AX] + num * ARG_SIZE, &cpu_storage)

#define _HLT                                            \
{                                                       \
	AsmCodeDestructor(&code_array);                     \
	CpuDestructor(&cpu_storage);                        \
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
	cpu_storage.registers[AX] = (arg) - 1 - ARG_SIZE;   \
}

#endif