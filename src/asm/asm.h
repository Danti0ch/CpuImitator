#ifndef ASM_H
#define ASM_H

#include "../cmd_constants.h"

// максимальная длина метки
const int MAX_LABEL_SIZE  = 25;
/// максимальное количество меток
const int MAX_LABELS_NUM  = 50;

/// яд для pc
const int INVALID_CMD_NUM = -1;

/// имена джампов
const char jump_names[N_JUMPS][MAX_INSTR_LEN] = {
    "jmp", "ja", "jb", "je", "jae", "jbe", "jne", "call"
};

/// количество проходов при ассемблировании
const int N_STAGES = 2;

/// коды ошибок
enum class ERROR_CODES{
    OK = 0,
    WRONG_REGISTER_NAME,
    WRONG_FORMAT,
    EMPTY_LINE,
    LABEL,
    WRONG_LABEL_NAME
};

// структура для хранения аргументов и их типа
struct cmd_arg{
    ARG_TYPE value[MAX_ARGS_NUM]; /// массив значений аргументов
    char type;                    /// тип аргументов заданный в виде битовой строки
};

/// структура для хранения информации о комманде
struct cmd_info_t{
    char        name[MAX_INSTR_LEN]; /// имя команды
    cmd_arg     arg;                 /// информация об аргументах комманды
    int         n_args;              /// количество аргументов
};

/**
 * компилирует текстовый файл asm_file_name в бинарник bin_file_name
 * 
 * \param asm_file_name имя исходного текстового файла
 * \param bin_file_name имя выходного бинарного файла
 * 
 */
void Compile(char const * const asm_file_name, char const * const bin_file_name);

#endif // ASM_H
