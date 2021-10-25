#ifndef COM_CONSTANTS
#define COM_CONSTANTS

const char VERSION 				= 1;

const int MAX_ARGS_NUM 			= 1;
const int ARG_SIZE 				= 4;

// необходимо ли?
const int MAX_INSTRUCTIONS_NUM 	= 20;
const int MAX_INSTR_LEN 		= 50;

// TODO: через хексы
const char RAM_POS         		= 0x80;
const char REG_POS 		  		= 0x40;
const char IMMED_CONST_POS 		= 0x20;
const char CMD_NUM_MASK	  		= 0x0F;

const int CMD_NAME_N_BIT  		= 4;

const char INVARIANT_SIGNATURE 	= 168;
const int N_SIGNATURES		  	= 3;

const char comment_symb 		= ';';

const int N_REGISTERS			= 8;

#define DEF_CMD(name, num, args, code)	\
CMD_##name = (num),

enum instructions_nums{
	#include "cmd_definitions.h"

	CMD_FICTIVE
};

#undef DEF_CMD

#endif