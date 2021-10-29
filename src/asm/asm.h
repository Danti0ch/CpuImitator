#ifndef ASM_H
#define ASM_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "text_storage.h"
#include "../cmd_constants.h"

const int MAX_LABEL_SIZE = 20;
const int INVALID_CMD_NUM = -1;

const int STAGE1 = 1;
const int STAGE2 = 2;

enum class ERROR_CODES{
	OK = 0,
	WRONG_REGISTER_NAME,
	WRONG_FORMAT,
	EMPTY_LINE,
	CMD_JUMP,
	LABEL,
	WRONG_JMP_ARG
};

struct _cmd_arg{
	int value[MAX_ARGS_NUM];
	char type;
};

struct _cmd_string{
	char 		name[MAX_INSTR_LEN];
	_cmd_arg 	arg;

	int 		n_args;
};

#define REGISTER_FORMAT_CHECK						\
													\
	if(cmd->arg.value[0]  >= N_REGISTERS){			\
		return ERROR_CODES::WRONG_REGISTER_NAME;	\
	}

void Compile(char const * const in_asm_name, char const * const out_asm_name);

#endif // ASM_H