// TODO: определиться со структуркой для процессора, кода и тп

#ifndef CPU_H
#define CPU_H

#include "my_stack.h"
#include "stdio.h"
#include "assert.h"
#include "../cmd_constants.h"
#include "windows.h"

const int RAM_SIZE    = 1024;
const int GPU_RAM_POS = 768;

const size_t STACK_INIT_SIZE  = 512;

const int RAM_CALLBACK_TIME	= 500;

const int AX = 0;
const int BX = 0;
const int CX = 0;
const int DX = 0;
const int EX = 0;
const int FX = 0;
const int GX = 0;
const int HX = 0;

struct cpu_t{
	stack_t stk;

	int registers[8];

	/// указатель на оперативную память
	int *p_ram;
};

struct asm_code{
	char *p;
	size_t len;
};

void Proccessing(char const * const asm_code_name);

#endif
