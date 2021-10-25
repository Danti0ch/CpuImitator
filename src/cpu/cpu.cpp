#include "cpu.h"

static void CpuConstructor(cpu_t* proc);
static asm_code get_asm_code(char const * const code_file_name);
static int get_arg(char* p_code, cpu_t* proc);
static void set_to_mem(char* p_code, cpu_t* proc, int arg);

#define DEF_CMD(name, num, args, code)					\
case CMD_##name:										\
	code;												\
	cpu_storage.registers[AX] += 1 + (args) * ARG_SIZE;	\
	break;

void Proccessing(char const * const asm_code_name){

	assert(asm_code_name != NULL);

	asm_code code_array = get_asm_code(asm_code_name);
	
	cpu_t cpu_storage = {};
	CpuConstructor(&cpu_storage);

	for(;;){
		switch((char)(code_array.p[cpu_storage.registers[AX]] & CMD_NUM_MASK)){
			#include "../cmd_definitions.h"
			default:
				assert(0 && "[#] wrong cmd num\n");
				break;
		}
	}
}

static asm_code get_asm_code(char const * const code_file_name){
	
	assert(code_file_name != NULL);

	asm_code code_array = {};

	FILE *code_file = fopen(code_file_name, "rb");

	assert(code_file != NULL);
	
	fseek(code_file, 0, SEEK_END);
	size_t file_size = ftell(code_file);
	fseek(code_file, 0, SEEK_SET);

	code_array.p = (char*)calloc(file_size, sizeof(char));
	code_array.len = file_size;

	// assert(fread(code_array.p, sizeof(char), file_size, code_file) != file_size);
	fread(code_array.p, sizeof(char), file_size, code_file);
	fclose(code_file);

	return code_array;
}

static void CpuConstructor(cpu_t* proc){

	assert(proc != NULL);

	assert(StackConstructor(&(proc->stk), STACK_INIT_SIZE) == ERROR_CODE::OK);

	proc->p_ram = (int*)calloc(RAM_SIZE, sizeof(int));

	proc->registers[AX] = N_SIGNATURES;

	assert(proc->p_ram != NULL);

	return;
}

static int get_arg(char* p_code, cpu_t* proc){
	
	assert(p_code != NULL);
	assert(proc != NULL);
	assert((char*)p_code != (char*)proc);

	int arg = 0;

	if(*p_code & IMMED_CONST_POS){
		arg = *(int*)(p_code + 1);
	}
	else if(*p_code & REG_POS){
		arg = proc->registers[*(p_code + 1)];
	}
	else{
		// TODO: запендюрить эту херню в макрос
		printf("%d: Wrong command format", proc->registers[AX]);
		assert(0);
	}
	if(*p_code & RAM_POS){
		Sleep(RAM_CALLBACK_TIME);
		assert( arg > 0 && arg < RAM_SIZE);
		arg = proc->p_ram[arg];
	}

	return arg;
}

static void set_to_mem(char* p_code, cpu_t* proc, int arg){
	
	assert(p_code != NULL);

	int ram_pos = 0;

	if(*p_code & IMMED_CONST_POS){
		ram_pos = *(int*)(p_code + 1);
	}
	else if(*p_code & REG_POS){
		ram_pos = proc->registers[*(p_code + 1)];
	}
	else{
		printf("%d: Wrong command format", proc->registers[AX]);
		assert(0);
	}

	if(*p_code & RAM_POS){
		Sleep(RAM_CALLBACK_TIME);
		assert( ram_pos > 0 && ram_pos < RAM_SIZE);
		proc->p_ram[ram_pos] = arg;
	}
	else if(*p_code & REG_POS){
		proc->registers[*(p_code + 1)] = arg;
	}
	else{
		printf("%d: Wrong command format", proc->registers[AX]);
		assert(0);
	}

	return;
}