#define __ ,

// DEF_CMD(name, num, args, parse_code)
DEF_CMD(HLT, 0, 0, 
{
	return;
})

DEF_CMD(OUT, 1, 0, 
{
	fprintf(stdout, "%d\n", StackTop(&cpu_storage.stk));
})

DEF_CMD(PUSH, 4, 1,
{
	assert(cpu_storage.registers[AX] + 1 < code_array.len);
	int _arg = get_arg(code_array.p + cpu_storage.registers[AX], &cpu_storage);
	StackPush((&cpu_storage.stk), _arg);
})

DEF_CMD(TOP, 5, 0,
{
	StackTop(&cpu_storage.stk);
})

DEF_CMD(POP, 6, 1,
{
	set_to_mem(code_array.p + cpu_storage.registers[AX], &cpu_storage, StackPop(&cpu_storage.stk));
})

DEF_CMD(ADD, 8, 0,
{
	int x = StackPop(&cpu_storage.stk);
	int y = StackPop(&cpu_storage.stk);

	StackPush(&cpu_storage.stk , x + y);
})

DEF_CMD(SUB, 9, 0,
{
	int x = StackPop(&cpu_storage.stk);
	int y = StackPop(&cpu_storage.stk);

	StackPush(&cpu_storage.stk , x - y);
})

DEF_CMD(MUL, 10, 0,
{
	int x = StackPop(&cpu_storage.stk);
	int y = StackPop(&cpu_storage.stk);

	StackPush(&cpu_storage.stk , x * y);
})

DEF_CMD(DIV, 11, 0,
{
	int x = StackPop(&cpu_storage.stk);
	int y = StackPop(&cpu_storage.stk);

	StackPush(&cpu_storage.stk , x / y);
})

DEF_CMD(SETPIX, 16, 1,
{

})
#undef __