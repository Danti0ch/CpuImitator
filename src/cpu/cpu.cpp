#include "cpu.h"

static void AsmCodeConstructor(asm_code* code_array, char const * const bin_file_name);

static void AsmCodeDestructor(asm_code* code_array);

static void CpuConstructor(cpu_t* cpu_storage);

static void CpuDestructor(cpu_t* cpu_storage);

static int get_arg(char* p_code, cpu_t* cpu_storage);

static void set_to_mem(char* p_code, cpu_t* cpu_storage, int arg);

static void change_pix_val(cpu_t* cpu_storage, int n_pix, int val);

static void show_graphic(cpu_t* cpu_storage);

static char get_pix_symb(cpu_t* cpu_storage, int x, int y);

#define DEF_CMD(name, num, args, code)                  \
case CMD_##name:                                        \
	code;                                               \
	cpu_storage.registers[AX] += 1 + (args) * ARG_SIZE; \
	break;

void Proccessing(char const * const bin_file_name){

	assert(bin_file_name != NULL);

	asm_code code_array = {};
	AsmCodeConstructor(&code_array, bin_file_name);

	cpu_t cpu_storage = {};
	CpuConstructor(&cpu_storage);

	if(code_array.p[0] != INVARIANT_SIGNATURE){
		LOG_ERROR_MSG(" signature is invalid");
	}
	if(code_array.p[1] != VERSION){
		LOG_ERROR_MSG("version is invalid");
	}

	for(;;){
		switch((char)(code_array.p[cpu_storage.registers[AX]] & CMD_NUM_MASK)){
			#include "../cmd_definitions.h"
			default:
				printf("ip = %d\n", cpu_storage.registers[AX]);
				LOG_ERROR_MSG("wrong cmd num");
				break;
		}
	}

	AsmCodeDestructor(&code_array);
	CpuDestructor(&cpu_storage);

	return;
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

static void CpuConstructor(cpu_t* cpu_storage){

	assert(cpu_storage != NULL);

	assert(StackConstructor(&(cpu_storage->stk), STACK_INIT_SIZE)        == ERROR_CODE::OK);
	assert(StackConstructor(&(cpu_storage->call_stack), STACK_INIT_SIZE) == ERROR_CODE::OK);

	cpu_storage->p_ram = (int*)calloc(RAM_SIZE, sizeof(int));
	assert(cpu_storage->p_ram != NULL);

	cpu_storage->p_graphic_ram = (char*)cpu_storage->p_ram + GPU_RAM_POS;

	memset(cpu_storage->p_graphic_ram, 0x00, GPU_RAM_SIZE);
	
	cpu_storage->registers[AX] = N_SIGNATURES;

	return;
}

static void CpuDestructor(cpu_t* cpu_storage){

	assert(cpu_storage != NULL);

	assert(StackDestructor(&(cpu_storage->stk))        == ERROR_CODE::OK);
	assert(StackDestructor(&(cpu_storage->call_stack)) == ERROR_CODE::OK);

	free(cpu_storage->p_ram);

	cpu_storage->p_ram 		= (int*)POISON_POINTER;
	cpu_storage->p_graphic_ram = (char*)POISON_POINTER;

	return;
}

static int get_arg(char* p_code, cpu_t* cpu_storage){
	
	assert(p_code != NULL);
	assert(cpu_storage != NULL);

	int arg = 0;

	if(*p_code & IMMED_CONST_POS){
		arg = *(int*)(p_code + 1);
	}
	else if(*p_code & REG_POS){
		arg = cpu_storage->registers[(int)*(p_code + 1)];
	}
	else{
		LOG_ERROR_MSG("wrong cmd type");
	}

	if(*p_code & RAM_POS){
		Sleep(RAM_CALLBACK_TIME);

		if( arg < 0 || arg > RAM_SIZE){
			LOG_ERROR_MSG("invalid ram position");
		}

		arg = cpu_storage->p_ram[arg];
	}

	return arg;
}

static void set_to_mem(char* p_code, cpu_t* cpu_storage, int arg){
	
	assert(p_code != NULL);

	int ram_pos = 0;

	if(*p_code & IMMED_CONST_POS){
		ram_pos = *(int*)(p_code + 1);
	}
	else if(*p_code & REG_POS){
		ram_pos = cpu_storage->registers[(int)*(p_code + 1)];
	}
	else{
		LOG_ERROR_MSG("invalid arg type");
	}

	if(*p_code & RAM_POS){
		Sleep(RAM_CALLBACK_TIME);

		if( ram_pos < 0 || ram_pos > RAM_SIZE){
			LOG_ERROR_MSG("invalid ram position");
		}
		cpu_storage->p_ram[ram_pos] = arg;
	}
	else if(*p_code & REG_POS){
		cpu_storage->registers[(int)*(p_code + 1)] = arg;
	}
	else{
		LOG_ERROR_MSG("invalid arg type");
	}

	return;
}

static void change_pix_val(cpu_t* cpu_storage, int n_pix, int val){

	assert(cpu_storage != NULL);

	if(n_pix > GPU_RAM_SIZE * N_BITS_IN_BYTE || n_pix < 0){
		LOG_ERROR_MSG("coordinate values are invalid");
	}

	int byte_offset = n_pix / N_BITS_IN_BYTE;
	int bit_offset  = n_pix % N_BITS_IN_BYTE;

	if(val == 0){
		*(cpu_storage->p_graphic_ram + byte_offset) &= (char)~(1 << bit_offset);
	}
	else{
		*(cpu_storage->p_graphic_ram + byte_offset) |= (char)(1 << bit_offset);
	}

	return;
}

static void show_graphic(cpu_t* cpu_storage){

	assert(cpu_storage != NULL);
	
	for(int y = 0; y < WINDOW_SIDE; y++){
		for(int x = 0; x < WINDOW_SIDE; x++){

			char pix = get_pix_symb(cpu_storage, x, y);
			printf("%c", pix);
		}
		printf("\n");
	}
	printf("\n_____________________________________________\n");
}

static char get_pix_symb(cpu_t* cpu_storage, int x, int y){

	assert(cpu_storage != NULL);
	assert((x < WINDOW_SIDE) && (y < WINDOW_SIDE));

	int byte_offset = (y * WINDOW_SIDE + x) / N_BITS_IN_BYTE;
	int bit_offset  = (y * WINDOW_SIDE + x) % N_BITS_IN_BYTE;

	char pix_val = *(cpu_storage->p_graphic_ram + byte_offset);

	if(pix_val & (1 << bit_offset)){
		return pix_1;
	}
	else{
		return pix_0;
	}
}
