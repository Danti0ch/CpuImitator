#include "dsl.h"

// DEF_CMD(name, num, args, parse_code)
DEF_CMD(HLT, 0, 0, 
{
	_HLT;
})

DEF_CMD(OUT, 1, 0, 
{	
	int x = _TOP;
	fprintf(stdout, "%d.%d\n", x / ARG_PRECISION, x % ARG_PRECISION);
})

DEF_CMD(IN, 2, 0,
{	
	double dx = 0;
	scanf("%lf", &dx);

	int x = (int)round(dx * ARG_PRECISION);

	_PUSH(x);
})

DEF_CMD(PUSH, 4, 1,
{
	_PUSH(_GET_ARG(0));
})

DEF_CMD(TOP, 5, 0,
{
	_TOP;
})

DEF_CMD(POP, 6, 1,
{
	_HLT_IF(set_to_mem(code_array.p + cpu_storage.registers[AX], &cpu_storage, _POP));
})

DEF_CMD(ADD, 8, 0,
{
	int x = _POP;
	int y = _POP;

	_PUSH(x + y);
})

DEF_CMD(SUB, 9, 0,
{
	int x = _POP;
	int y = _POP;

	_PUSH(x - y);
})

DEF_CMD(MUL, 10, 0,
{
	int x = _POP;
	int y = _POP;

	_PUSH(x * y / ARG_PRECISION);
})

DEF_CMD(DIV, 11, 0,
{
	int x = _POP;
	int y = _POP;

	_PUSH(x / y * ARG_PRECISION);
})

DEF_CMD(ROOT, 12, 0,
{
	int x = _POP;
	double dx = x;

	dx /= ARG_PRECISION;

	dx = sqrt(dx);

	if(fabs(dx) <= EPS){
		_PUSH(0);
	}
	else{
		x = (int)round(dx * ARG_PRECISION);
		_PUSH(x);
	}
})

DEF_CMD(SETPIX, 16, 1,
{
	int n_pix = _GET_ARG(0);

	_HLT_IF(change_pix_val(&cpu_storage, n_pix));
})

DEF_CMD(DRAW, 17, 0,
{
	show_graphic(&cpu_storage);
})

DEF_CMD(JMP, 20, 1,
{
	_JMP(_GET_ARG(0));
})

DEF_CMD(JA, 21, 1,
{	
	int x = _POP;
	int y = _POP;

	if(x < y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(JB, 22, 1,
{	
	int x = _POP;
	int y = _POP;

	if(x > y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(JE, 23, 1,
{	
	int x = _POP;
	int y = _POP;

	if(x == y){
		_JMP(_GET_ARG(0));
	}
})


DEF_CMD(JAE, 24, 1,
{	
	int x = _POP;
	int y = _POP;

	if(x <= y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(JBE, 25, 1,
{	
	int x = _POP;
	int y = _POP;

	if(x >= y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(JNE, 26, 1,
{	
	int x = _POP;
	int y = _POP;

	if(x != y){
		_JMP(_GET_ARG(0));
	}
})


DEF_CMD(CALL, 27, 1,
{	
	// TODO: разборки с адресованием
	StackPush(&(cpu_storage.call_stack), cpu_storage.registers[AX] + 1 + ARG_SIZE);
	int function_pos = _GET_ARG(0);

	_JMP(function_pos);
})

DEF_CMD(RET, 28, 0,
{
	int call_function_pos = StackPop(&(cpu_storage.call_stack));
	// TODO: разборки с адресом
	_JMP(call_function_pos + ARG_SIZE);
})

// TODO: сделать смешную комманду
