#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <math.h>
#include "my_stack.h"
#include "../cmd_constants.h"

const int RAM_SIZE              = 4096;

const int N_BITS_IN_BYTE        = 8;
const int WINDOW_SIDE           = 32;
const int GPU_RAM_SIZE          = WINDOW_SIDE * WINDOW_SIDE / N_BITS_IN_BYTE;
const int GPU_RAM_POS           = RAM_SIZE - GPU_RAM_SIZE;

const char pix_0                = ' ';
const char pix_1                = '*';

const size_t STACK_INIT_SIZE    = 512;

const int RAM_CALLBACK_TIME     = 1;

const int64_t POISON_POINTER = 7;

const int AX = 0;
const int BX = 1;
const int CX = 2;
const int DX = 3;
const int EX = 4;
const int FX = 5;
const int GX = 6;
const int HX = 7;

enum class ERROR_CODES{
    OK = 0,
    NOT_OK
};

struct cpu_t{
    stack_t stk;

    stack_t call_stack;

    int registers[8];

    /// указатель на оперативную память
    int *p_ram;

    char *p_graphic_ram;
};

struct asm_code{
    char *p;
    size_t len;
};

#define LOG_ERROR_MSG(msg)          \
                                    \
    printf("ERROR: %s\n", (msg));   \
                                    \
    assert(0);

void Proccessing(char const * const bin_file_name);

#endif
