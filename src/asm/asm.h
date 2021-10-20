#ifndef ASM_H
#define ASM_H

#include "com_constants.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "text_storage.h"

// необходимо ли?
const int MAX_INSTRUCTIONS_NUM = 20;

// TODO: перенести к файлу централизации
const int MAX_ARGS_NUM = 2;
const int ARG_SIZE = 4;

void Assembling(char const * const asm_file_name);

long int get_file_len(FILE *ptr_file);

char* get_buffer_from_file(char const * const file_name);

#endif // ASM_H