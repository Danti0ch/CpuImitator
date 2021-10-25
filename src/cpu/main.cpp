#include <stdio.h>
#include "cpu.h"

int main(const int argc, char const * const argv[]){

	open_log_file("log.txt");
	Proccessing("asm.bin");
	close_log_file();
	return 0;
}
