#include "dsl.h"

// DEF_CMD(name, num, args, parse_code)
DEF_CMD(hlt, 0, 0, 
{
	_HLT;
})

DEF_CMD(out, 1, 0, 
{	
	ARG_TYPE x = _TOP;
	printf("%d.%d\n", x / ARG_PRECISION, abs(x) % ARG_PRECISION);
})

DEF_CMD(in, 2, 0,
{	
	double dx = 0;
	scanf("%lf", &dx);

	ARG_TYPE x = (ARG_TYPE)round(dx * ARG_PRECISION);

	_PUSH(x);
})

DEF_CMD(push, 4, 1,
{
	_PUSH(_GET_ARG(0));
})

DEF_CMD(top, 5, 0,
{
	_TOP;
})

DEF_CMD(pop, 6, 1,
{
	set_to_mem(code_array.p + cpu_storage.registers[PC], &cpu_storage, _POP);
})

DEF_CMD(add, 8, 0,
{
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;
	_PUSH(x + y);
})

DEF_CMD(sub, 9, 0,
{
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	_PUSH(x - y);
})

DEF_CMD(mul, 10, 0,
{
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	_PUSH(x * y / ARG_PRECISION);
})

DEF_CMD(div, 11, 0,
{
	double x = (double)_POP;
	double y = (double)_POP;

	_PUSH((ARG_TYPE)round(x / y * ARG_PRECISION));
})

DEF_CMD(root, 12, 0,
{
	ARG_TYPE x = _POP;
	double dx = x;

	dx /= ARG_PRECISION;

	dx = sqrt(dx);

	if(fabs(dx) <= EPS){
		_PUSH(0);
	}
	else{
		x = (ARG_TYPE)round(dx * ARG_PRECISION);
		_PUSH(x);
	}
})

DEF_CMD(round, 13, 0,
{	
	ARG_TYPE x = (ARG_TYPE)round(((double)_POP) / ((double)ARG_PRECISION)) * ARG_PRECISION;
	_PUSH(x);
})

DEF_CMD(setpix, 16, 1,
{	
	ARG_TYPE val = (int)(_GET_ARG(0) / ARG_PRECISION);

	int x = (int)(_POP / ARG_PRECISION);
	int y = (int)(_POP / ARG_PRECISION);
	
	int n_pix = y * WINDOW_SIDE + x;

	change_pix_val(&cpu_storage, n_pix, val);
})

DEF_CMD(draw, 17, 0,
{
	show_graphic(&cpu_storage);
})

DEF_CMD(jmp, 20, 1,
{
	_JMP(_GET_ARG(0));
})

DEF_CMD(ja, 21, 1,
{	
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	if(x < y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(jb, 22, 1,
{	
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	if(x > y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(je, 23, 1,
{	
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	if(x == y){
		_JMP(_GET_ARG(0));
	}
})


DEF_CMD(jae, 24, 1,
{	
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	if(x <= y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(jbe, 25, 1,
{	
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	if(x >= y){
		_JMP(_GET_ARG(0));
	}
})

DEF_CMD(jne, 26, 1,
{	
	ARG_TYPE x = _POP;
	ARG_TYPE y = _POP;

	if(x != y){
		_JMP(_GET_ARG(0));
	}
})


DEF_CMD(call, 27, 1,
{	
	StackPush(&(cpu_storage.call_stack), cpu_storage.registers[PC] + 1 + ARG_SIZE);
	ARG_TYPE function_pos = _GET_ARG(0);

	_JMP(function_pos);
})

DEF_CMD(ret, 28, 0,
{
	ARG_TYPE call_function_pos = StackPop(&(cpu_storage.call_stack));
	_JMP(call_function_pos + ARG_SIZE);
})
