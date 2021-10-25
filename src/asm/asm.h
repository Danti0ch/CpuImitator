#ifndef ASM_H
#define ASM_H

#include "../cmd_constants.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "text_storage.h"

// норм?
struct _cmd_arg{
	int value;
	char type;	
};

struct _cmd_string{
	char 		name[MAX_INSTR_LEN];
	_cmd_arg 	arg;

	int 		n_args;
};

void Assembling(char const * const in_asm_name, char const * const out_asm_name);

#endif // ASM_H