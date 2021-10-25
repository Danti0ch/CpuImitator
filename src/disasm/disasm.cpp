
#include "asm.h"

// TODO: добавить регистры и опер память, парсинг параметров
// TODO: optional [переменное количество параметров]

#define PREPROCCESOR_TO_STRING(name) #name

#define DEF_CMD(name, num, args, code)							\
	if(strcmp(PREPROCCESOR_TO_STRING(name)__ cmd_name) == 0){	\
																\
		printf("%d\n", num);									\
		code_array[ip++] = 16;									\
																\
		if(n_readen > 1){										\
			code_array[ip++] = cmd_arg;							\
		}														\
	}															\
	else

// TODO: сделать без буферизации, замерить время
// TODO: добавить функцию для обработки строки, корректность ввода, убрать комментарии и тп
void Disassembling(char const * const code_name, char const * const asm_name){

	assert(in_asm_name != NULL);
	assert(out_asm_name != NULL);

	// TODO: придумать лучшее имя
	asm_code *code_array = get_asm_code(code_name);
	int ip = 0;

	for(int n_cmd = 0; n_cmd < code_array.len; n_cmd++){

		char cmd_name[MAX_INSTR_LEN] = "";

		int cmd_arg = 0;

		int n_readen = sscanf(asm_content.p_lines[n_cmd].pointer, "%[A-Z] %d\n", cmd_name, &cmd_arg);

		printf("%d %s \n", cmd_arg, cmd_name);

		#include "../cmd_definitions.h"
		/* else */{
			printf("line %d: wrong format", n_cmd);
			assert(0);
		}
	}

	clear_mem_storage(&asm_content);

	FILE* code_file = fopen(out_asm_name, "wb");
	fwrite(code_array, sizeof(char), ip, code_file);
	fclose(code_file);

	return;
}


// TODO: прога для функций работы с файлами
asm_code* get_asm_code(char const * const code_file_name){
	
	assert(asm_file_name != NULL);

	asm_code *code_array = NULL;

	FILE *code_file = fopen(code_file_name, "rb");

	assert(code_file != NULL);
	
	fseek(code_file, 0, SEEK_END);
	size_t file_size = ftello(code_file);
	fseek(code_file, 0, SEEK_SET);

	code_array.p = (char*)calloc(file_size, sizeof(char));
	code_array.len = file_size;

	assert(fread(code_array, sizeof(char), file_size, code_file) != file_size);

	fclose(code_file);

	return code_array
}