#include <stdio.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "../cmd_constants.h"

int main(const int argc, char const * const argv[]){

	struct stat asm_stat;
	struct stat code_stat;

	if(argc != 2){
		printf("format should be: ./pseudo_make.exe [file_name].txt\n");
		return 0;
	}

	stat(argv[1], &asm_stat);
	stat(BIN_FILE_NAME, &code_stat);

	if(code_stat.st_mtime < asm_stat.st_mtime){
		
		printf("file %s has changed, recompile\n", argv[1]);

		char system_call_string[50] = "./asm.exe ";
		
		strcat(system_call_string, argv[1]);
		system(system_call_string);
	}

	system("./cpu.exe");
	return 0;
}
