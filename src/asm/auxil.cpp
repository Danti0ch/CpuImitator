#include "auxil.h"

void Console_argv_proccesing(const int argc, char const * const argv[], char* asm_file_name){

	if(argc < 2){
		return;
	}

	// fix
	assert(strlen(argv[1]) < MAX_ASM_FILE_LEN);
	strcpy(asm_file_name, argv[1]);

	return;
}