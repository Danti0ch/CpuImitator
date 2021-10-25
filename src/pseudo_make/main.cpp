#include <stdio.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(const int argc, char const * const argv[]){

	struct stat asm_stat;
	struct stat code_stat;

	stat("asm.txt", &asm_stat);
	stat("asm.bin", &code_stat);

	if(code_stat.st_mtime < asm_stat.st_mtime){
		printf("file asm.txt has changed, recompile\n");
		system("asm.exe");
	}

	system("cpu.exe");
	return 0;
}
