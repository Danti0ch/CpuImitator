#include "cpu.h"

// TODO: fix out with size = 0

static void AsmCodeConstructor(asm_code* code_array, char const * const bin_file_name);

static void AsmCodeDestructor(asm_code* code_array);

static void CpuConstructor(cpu_t* proc);

static void CpuDestructor(cpu_t* proc);

static int get_arg(char* p_code, cpu_t* proc);

static void set_to_mem(char* p_code, cpu_t* proc, int arg);

static ERROR_CODES change_pix_val(cpu_t* proc, int n_pix);

static void show_graphic(cpu_t* proc);

static char get_pix_symb(cpu_t* proc, int x, int y);

#define DEF_CMD(name, num, args, code)					\
case CMD_##name:										\
	code;												\
	cpu_storage.registers[AX] += 1 + (args) * ARG_SIZE;	\
	break;

void Proccessing(char const * const bin_file_name){

	assert(bin_file_name != NULL);

	asm_code code_array = {};
	AsmCodeConstructor(&code_array, bin_file_name);

	cpu_t cpu_storage = {};
	CpuConstructor(&cpu_storage);

	// TODO: проверка сигнатуры и версии
	// TODO: пофиксить setpix
	// TODO: пофиксить draw

	for(;;){
		switch((char)(code_array.p[cpu_storage.registers[AX]] & CMD_NUM_MASK)){
			#include "../cmd_definitions.h"
			default:
				assert(0 && "[#] wrong cmd num\n");
				break;
		}
	}
}

static void AsmCodeConstructor(asm_code* code_array, char const * const bin_file_name){
	
	assert(bin_file_name != NULL);

	FILE *code_file = fopen(bin_file_name, "rb");

	assert(code_file != NULL);
	
	fseek(code_file, 0, SEEK_END);
	size_t file_size = ftell(code_file);
	fseek(code_file, 0, SEEK_SET);

	code_array->p = (char*)calloc(file_size, sizeof(char));
	assert(code_array->p != NULL);

	code_array->len = file_size;

	assert(fread(code_array->p, sizeof(char), file_size, code_file) == file_size);
	fclose(code_file);

	return;
}

static void AsmCodeDestructor(asm_code* code_array){

	assert(code_array != NULL);

	free(code_array->p);
	code_array->p = (char*)POISON_POINTER;
	code_array->len = 0;

	return;
}

static void CpuConstructor(cpu_t* proc){

	assert(proc != NULL);

	assert(StackConstructor(&(proc->stk), STACK_INIT_SIZE)        == ERROR_CODE::OK);
	assert(StackConstructor(&(proc->call_stack), STACK_INIT_SIZE) == ERROR_CODE::OK);

	proc->p_ram = (int*)calloc(RAM_SIZE, sizeof(int));
	assert(proc->p_ram != NULL);

	proc->p_graphic_ram = (char*)proc->p_ram + GPU_RAM_POS;

	memset(proc->p_graphic_ram, 0x00, GPU_RAM_SIZE);
	
	proc->registers[AX] = N_SIGNATURES;

	return;
}

static void CpuDestructor(cpu_t* proc){

	assert(proc != NULL);

	assert(StackDestructor(&(proc->stk))        == ERROR_CODE::OK);
	assert(StackDestructor(&(proc->call_stack)) == ERROR_CODE::OK);

	free(proc->p_ram);

	proc->p_ram 		= (int*)POISON_POINTER;
	proc->p_graphic_ram = (char*)POISON_POINTER;

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
		WRONG_CMD_MSG
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
		WRONG_CMD_MSG
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
		WRONG_CMD_MSG
	}

	return;
}

static ERROR_CODES change_pix_val(cpu_t* proc, int n_pix){

	assert(proc != NULL);


	if(n_pix > GPU_RAM_SIZE * N_BITS_IN_BYTE || n_pix < 0){
		printf("ERROR: the coordinates must not exceed the value %d\n", WINDOW_SIDE);
		return ERROR_CODES::NOT_OK;
	}

	int byte_offset = n_pix / N_BITS_IN_BYTE;
	int bit_offset  = n_pix % N_BITS_IN_BYTE;

	*(proc->p_graphic_ram + byte_offset) ^= 1 << bit_offset;

	return ERROR_CODES::OK;
}

// TODO: побайтовый вывод
// возможно нужно сделать архитектуру под построковый вывод? или ускорение неважно?
static void show_graphic(cpu_t* proc){

	assert(proc != NULL);
	
	for(int y = 0; y < WINDOW_SIDE; y++){
		for(int x = 0; x < WINDOW_SIDE; x++){

			int bit_offset = y * WINDOW_SIDE + x;

			char pix = get_pix_symb(proc, x, y);
			printf("%c", pix);
		}
		printf("\n");
	}
	printf("\n_____________________________________________\n");
}

static char get_pix_symb(cpu_t* proc, int x, int y){

	assert(proc != NULL);

	assert((x < WINDOW_SIDE) && (y < WINDOW_SIDE));

	int byte_offset = (y * WINDOW_SIDE + x) / N_BITS_IN_BYTE;
	int bit_offset  = (y * WINDOW_SIDE + x) % N_BITS_IN_BYTE;

	char byte = *(proc->p_graphic_ram + byte_offset);

	if(byte & (1 << bit_offset)){
		return pix_1;
	}
	else{
		return pix_0;
	}
}
