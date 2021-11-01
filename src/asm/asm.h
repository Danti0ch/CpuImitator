#ifndef ASM_H
#define ASM_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "text_storage.h"
#include "../cmd_constants.h"

// максимальная длина метки
const int MAX_LABEL_SIZE  = 20;

const int INVALID_CMD_NUM = -1;

const char jump_names[N_JUMPS][MAX_INSTR_LEN] = {
    "jmp", "ja", "jb", "je", "jae", "jbe", "jne", "call"
};

const int N_STAGES = 2;

enum class ERROR_CODES{
    OK = 0,
    WRONG_REGISTER_NAME,
    WRONG_FORMAT,
    EMPTY_LINE,
    LABEL,
    WRONG_LABEL_NAME
};

// структура для хранения аргументов и типа команды

struct cmd_arg{
    int value[MAX_ARGS_NUM];
    char type;
};

// структура для хранения информации о комманде
struct _cmd_info{
    char        name[MAX_INSTR_LEN];
    cmd_arg     arg;

    int         n_args;
};

#define REGISTER_FORMAT_CHEC                        \
                                                    \
    if(cmd_info->arg.value[0]  >= N_REGISTERS){     \
        return ERROR_CODES::WRONG_REGISTER_NAME;    \
    }

#define LOG_ERROR_MSG(msg)                                                      \
                                                                                \
    printf("ERROR from file (%s): %s\n"                                         \
           "\t[%u]: %s\n",                                                      \
           asm_file_name, (msg), n_line + 1, asm_text.p_lines[n_line].pointer); \
    destruct_code_array(code_array);

void Compile(char const * const in_asm_name, char const * const out_asm_name);

#endif // ASM_H
