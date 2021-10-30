#ifndef COM_CONSTANTS
#define COM_CONSTANTS

#define meow printf("meeeeeooooow\n");

const char VERSION 				= 1;

const int MAX_ARGS_NUM 			= 2;
const int ARG_SIZE 				= sizeof(int);
const int MAX_INSTRUCTIONS_NUM 	= 500;
const int MAX_INSTR_LEN 		= 100;

const int ARG_PRECISION 		= 1000;
const double EPS = 1e-6;

const char RAM_POS         		= 0x80;
const char REG_POS 		  		= 0x40;
const char IMMED_CONST_POS 		= 0x20;
const char CMD_NUM_MASK	  		= 0x1F;

const char INVARIANT_SIGNATURE 	= 168;
const int N_SIGNATURES		  	= 3;

const char comment_symb 		= ';';

const int N_REGISTERS			= 8;

const int MIN_JMP_NUM			= 20;
const int MAX_JMP_NUM 			= 27;
const int N_JUMPS				= MAX_JMP_NUM - MIN_JMP_NUM + 1;

// note: JUMP CODES IS 20 - 27

#define DEF_CMD(name, num, args, code)	\
CMD_##name = (num),

enum instructions_nums{
	#include "cmd_definitions.h"

	CMD_FICTIVE
};

#undef DEF_CMD

#endif