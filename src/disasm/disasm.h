#ifndef ASM_H
#define ASM_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../cmd_constants.h"
#include "log.h"

const char DEFAULT_LOG_NAME[20] = "lst.txt";

struct asm_code{
	char *p;
	size_t len;
};


void Disassembling(char const * const bin_file_name, char const * const log_file_name);

#endif // ASM_H