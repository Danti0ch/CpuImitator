#include <stdio.h>
#include "asm.h"

int main(int const argc, char const * const argv[]){

	if(argc == 2){
		Compile(argv[1], BIN_FILE_NAME);
	}
	else{
		printf("format should be: asm.exe [file_name].txt\n");
	}

	return 0;
}
