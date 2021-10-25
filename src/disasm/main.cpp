#include <stdio.h>
#include "disasm.h"

int main(const int argc, char const * const argv[]){

	Assembling("../../build/asm.bin", "../../build/disasm.txt");
	return 0;
}
