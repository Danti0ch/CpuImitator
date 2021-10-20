#ifndef COM_CONSTANTS
#define COM_CONSTANTS


/*
// плохо
const int N_INSTRUCTIONS 	  = 7;

const int MAX_LEN_INSTRUCTION = 20;
*/

const int ARG_SIZE = 4;

enum instructions_nums{
	HLT 	= 0,
	OUT 	= 1,
	
	PUSH 	= 20,

	ADD 	= 40,
	SUB 	= 41,
	MUL 	= 42,
	DIV 	= 43

};

/*
const char INTRUCTION_NAMES[N_INSTRUCTIONS][N_INSTRUCTIONS];
INTRUCTION_NAMES[HLT]
*/

#endif