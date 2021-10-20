#ifndef AUXIL_H
#define AUXIL_H

#include <assert.h>
#include <string.h>

#define DEFAULT_ASM_FILE_NAME "asm.txt"
const int MAX_ASM_FILE_LEN 	  = 1000;

void Console_argv_proccesing(const int argc, char const * const argv[], char* asm_file_name);

#endif