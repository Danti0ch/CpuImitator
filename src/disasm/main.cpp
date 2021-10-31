#include <stdio.h>
#include "disasm.h"

int main(const int argc, char const * const argv[]){

	if(argc == 2){
		Disassembling(argv[1], DEFAULT_LOG_NAME);
	}
	else{
		printf("format should be: disasm.exe [file_name].bin\n");
	}
	
	return 0;
}
