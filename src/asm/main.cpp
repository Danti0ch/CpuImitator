#include <stdio.h>
#include "asm.h"

int main(const int argc, char const * const argv[]){

	Compile("asm.txt", "asm.bin");
	return 0;
}
