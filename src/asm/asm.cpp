
#include "asm.h"

// TODO: сделать без буферизации, замерить время
void Assembling(char const * const asm_file_name){

	assert(asm_file_name != NULL);

	char* buffer = get_buffer_from_file(asm_file_name);

	// TODO: придумать лучшее имя
	char *code_array = calloc((MAX_ARGS_NUM + 1) * ARG_SIZE * MAX_INTSTRUCTIONS_NUM);

	for(int i = 0;)
	return;
}


// O(n)
// TODO: подумать о синтаксической обработке корректности кода
void prepare_asm_buffer(char *buffer, long int buffer_size){

	assert(buffer != NULL);
	assert(buffer_size > 0);

	char *p_new, *p_last;

	for(p_new = buffer, p_last = buffer; p_last < buffer + buffer_size; p_new++, p_last++){

		if(p_last == ';'){
			while((p_last < buffer + buffer_size) && (p_last != '\n')){
				p_last++;
			}
		}

	}
}


// TODO: error_codes ???
char* get_buffer_from_file(char const * const file_name){

	assert(file_name != NULL);

	FILE *ptr_file = fopen(file_name, "r");
	assert(ptr_file != NULL);

	long int buf_size = get_file_len(ptr_file);
	char* buffer = (char*)calloc(buf_size, sizeof(char));
	assert(buffer != NULL);

	fread(buffer, sizeof(char), buf_size, ptr_file);

	fclose(ptr_file);

	return buffer;
}

// TODO: вынести в отдельный файл, можно(нужно) ли пофиксить то, что файл возвращается в начальное положение

long int get_file_len(FILE *ptr_file){

	assert(ptr_file != NULL);

	fseek(ptr_file , 0 , SEEK_END);
  	long int file_len = ftell(ptr_file);
	fseek(ptr_file , 0 , SEEK_SET);

	return file_len;
}
