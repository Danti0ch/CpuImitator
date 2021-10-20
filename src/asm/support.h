/**
 * \file
 * 
 * \brief файл содержит обьявления вспомогательных функций и констант, необходимых в других файлах
 * 
 */

#ifndef SUPP_H
#define SUPP_H

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/// коды ошибок функций
enum func_codes{ 
    MEM_ALLOC_ERROR       = -100,  ///< ошибка обращения к памяти
    EMPTY_FILE            = -99,   ///< ошибка работы с пустым файлом
    IDENTICAL_POINTERS    = -98,   ///< идентичные указатели
    OK                    =  1     ///< успешное завершение
};

/// максимальная длина строки для чтения из файла
const int MAX_README_LINE_LEN = 1000;

/// максимальная длина строки для чтения из файла
const int MAX_INPUT_FILE_NAME_LEN = 100;

/// ядовитые указатели
enum poison_pointers{
    P_AFTER_CALLOC = 2
};

/// выводит содержимое файла "readme.txt" в консоль
void show_help(void);

/**
 *  меняет значения обьектов на которые указывают p_elem1, p_elem2.
 * Обьекты должны быть одного размера.
 * 
 * \param p_elem1, p_elem2 указатели на обьекты, которые надо поменять
 * \param size_elem размер обьекта p_elem1(p_elem2)
 * 
 * \return код возвращаемого значения из func_codes
 */
enum func_codes swap(void *p_elem1, void *p_elem2, const size_t size_elem);

/**
 * получает количество строк и символов файла
 * 
 * \param file_name имя файла, размер которого нужно узнать
 * \param n_lines количество строк в файле
 * \param len количество символов в файле
 * 
 * \return код возвращаемого значения из func_codes
 */
enum func_codes get_file_len(const char *file_name, size_t *n_lines, size_t *len);

/**
 * выводит содержимое текстового файла в стандартный поток вывода
 * 
 * \param input_file файл, содержимое которого нужно вывести
 * 
 * \return код возвращаемого значения из func_codes
 */
enum func_codes display_file_content(FILE *input_file);

/**
 * находит количество байт между указателями
 * 
 * \param left_pointer, right_pointer границы отрезка памяти
 * 
 * \return количество байт
 */
inline size_t get_bytes(void *left_pointer, void *right_pointer);

/** 
 * проверяет, является ли char числом или буквой(английской или русской)
 * 
 */
int isalnum_rus(int c);

/**
 * считывает информацию о параметрах выполнения проекта из консоли
 * 
 */
int get_console_parms(const int argc, char const * const argv[], char* file_name, int *is_alnum_sorting);

#endif