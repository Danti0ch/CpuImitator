/**
 * \file
 * \brief файл содержит структуру для работы с группой строк, ее методы
 */

#ifndef TEXT_STORAGE_H
#define TEXT_STORAGE_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "support.h"

/// флаги, указывающие компаратору режим сортировки
enum sort_flags{
    STRAIGHT,       ///< прямая
    REVERSE,        ///< обратная
    ALNUM,          ///< прямая, учитываются только буквы и цифры
    ALNUM_REVERSE   ///< обратная, учитываются только буквы и цифры
};

/// хранит указатель на си строку и её длину
struct string{
    char *pointer;       ///< указатель на начало строки
    size_t len;          ///< длина строки (strlen)
};

struct text_storage{ /// структура, необходимая для хранения группы строк
    size_t len_buf;       ///< количество символов в буфере
    size_t num_lines;     ///< количество строк

    string *p_lines;      ///< массив строк типа string
    char *buffer;         ///< буфер, в котором находится содержимое всех строк. каждая строка должна оканчиваться '\0'
};

/**
 * считывает содержимое файла file_name в структуру storage
 * 
 * \param file_name имя файла
 * \param storage структура, в которую нужно считать содержимое
 * 
 * \return код возвращаемого значения из func_codes
 */
enum func_codes get_text_storage(const char *file_name, text_storage *storage);

/**
 * записывает в файл output_file строки из структуры storage, в порядке массива p_lines
 * 
 * \param output_file файл, в который нужно произвести запись
 * \param storage структура text_storage
 * 
 * \return код возвращаемого значения из func_codes
 */
enum func_codes write_storage(FILE *output_file, text_storage *storage);

/**
 * записывает в файл output_file символьный массив(буфер) из storage
 * 
 * \param output_file файл, в который будет происходить запись
 * \param storage структура text_storage, буфер которой нужно записать в файл
 * 
 * \return код возвращаемого значения из func_codes
 */
enum func_codes write_buffer_of_storage(FILE *output_file, text_storage *storage);

/**
 * функция очищает память, которая была динамически присвоена элементам storage в create_mem_storage
 * 
 * \param storage структура text_storage, память которой нужно очистить
 * 
 * \return код возвращаемого значения из func_codes
 */ 
enum func_codes clear_mem_storage(text_storage *storage);

/**
 * функция выделяет память для указателей storage. размер необходимый памяти задаётся storage.buf_len и storage.num_lines
 * 
 * \param storage структура text_storage для которой нужно выделить память
 * 
 * \return код возвращаемого значения из func_codes
 */ 
enum func_codes create_mem_storage(text_storage *storage);

/**
 * обертка для string_cmp с режимом STRAIGHT
 */
int string_cmp_straight(const void*str1, const void*str2);

/**
 * обертка для string_cmp с режимом REVERSE
 */
int string_cmp_reverse(const void*str1, const void*str2);

/**
 * обертка для string_cmp с режимом ALNUM
 */
int string_cmp_alnum(const void*str1, const void*str2);

/**
 * обертка для string_cmp с режимом ALNUM_REVERSE
 */
int string_cmp_alnumReverse(const void*str1, const void*str2);

/**
 * компаратор, сравнивает две строки(структуры) типа string
 * 
 * \param str1, str2 string структуры, которые нужно сравнить
 * \param mode режим сортировки, заданный константной из набора sort_flags
 * 
 * \return  1 если str1 > str2
 * \return -1 если str1 < str2
 * \return  0 если str1 == str2
 */
int string_cmp(const string *str1, const string *str2, enum sort_flags mode);

#endif