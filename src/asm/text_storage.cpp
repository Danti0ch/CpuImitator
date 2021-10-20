/**
 * \file
 * \brief  файл содержит определения функций, объявленных в text_storage.h
 *
 */

#include "text_storage.h"

enum func_codes clear_mem_storage(text_storage *storage){

    assert(storage != NULL);
    
    assert(storage->p_lines != NULL);
    assert(storage->buffer != NULL);

    free(storage->p_lines);
    free(storage->buffer);

    storage->buffer = (char*)P_AFTER_CALLOC;
    storage->p_lines = (string*)P_AFTER_CALLOC;
    
    return OK;
}

enum func_codes create_mem_storage(text_storage *storage){

    assert(storage != NULL);

    assert(storage->len_buf > 0);
    assert(storage->num_lines > 0);
    
    storage->buffer = (char*)calloc(storage->len_buf, sizeof(char));

    storage->p_lines = (string*)calloc(storage->num_lines, sizeof(string));

    if((storage->buffer == NULL) || (storage->p_lines == NULL)){
        return MEM_ALLOC_ERROR;
    }

    return OK;
}

enum func_codes get_text_storage(const char *file_name, text_storage *storage){

    assert(storage != NULL);
    
    assert(file_name != NULL);

    enum func_codes getting_len_result = get_file_len(file_name, &(storage->num_lines), &(storage->len_buf));

    if(getting_len_result != OK){
        return getting_len_result;
    }

    if(storage->len_buf == 0){
        return EMPTY_FILE;
    }

    enum func_codes mem_for_storage_result = create_mem_storage(storage);

    if(mem_for_storage_result != OK){
        return mem_for_storage_result;
    }

    FILE *input_file = fopen(file_name, "r");

    assert(input_file != NULL);

    int reading_status = fread(storage->buffer, sizeof storage->buffer[0], storage->len_buf, input_file);
    assert(reading_status >= 0);

    fclose(input_file);

    storage->buffer[storage->len_buf - 1] = '\n';

    int is_new_line = 1;
    size_t num_line = 0;
    size_t num_symbols_in_line = 1;

    for(int ind_buf = 0; ind_buf < storage->len_buf; ind_buf++, num_symbols_in_line++){

        if(is_new_line){
            storage->p_lines[num_line].pointer = &(storage->buffer[ind_buf]);

            is_new_line = 0;
        }
        if(storage->buffer[ind_buf] == '\n'){
            storage->p_lines[num_line].len = num_symbols_in_line - 1; // '\0' не учитывается в длине строки

            num_symbols_in_line = 0;
            storage->buffer[ind_buf] = '\0';

            is_new_line = 1;
            num_line++;
        }
    }
    return OK;
}


enum func_codes write_storage(FILE *output_file, text_storage *storage){

    assert(storage != NULL);
    assert(output_file != NULL);

    for(int i = 0; i < storage->num_lines; i++){
        fputs(storage->p_lines[i].pointer, output_file);
        fputc('\n', output_file);
    }
    return OK;
}

enum func_codes write_buffer_of_storage(FILE *output_file, text_storage *storage){

    assert(storage != NULL);
    assert(output_file != NULL);

    char *n_symb = storage->buffer;

    for(int n_line = 0; n_line < storage->num_lines; n_line++, n_symb++){
        for(; *n_symb != '\0'; n_symb++){
            fputc(*n_symb, output_file);
        }
        fputc('\n', output_file);
    }

    return OK;
}

int string_cmp_straight(const void*str1, const void*str2){
    return string_cmp((const string *)str1, (const string *)str2, STRAIGHT);    
}    

int string_cmp_reverse(const void*str1, const void*str2){
    return string_cmp((const string *)str1, (const string *)str2, REVERSE);    
}      

int string_cmp_alnum(const void*str1, const void*str2){
    return string_cmp((const string *)str1, (const string *)str2, ALNUM);    
}    

int string_cmp_alnumReverse(const void*str1, const void*str2){
    return string_cmp((const string *)str1, (const string *)str2, ALNUM_REVERSE);    
}    

int string_cmp(const string *str1, const string *str2, enum sort_flags mode){

    assert(str1->pointer != NULL);
    assert(str2->pointer != NULL);

    if(str1->pointer == str2->pointer){
        return 0;
    }

    assert(str1->len >= 0);
    assert(str2->len >= 0);


    char *str1_stepper = str1->pointer;
    char *str2_stepper = str2->pointer;
    
    char *str1_beg = str1->pointer;
    char *str2_beg = str2->pointer;
    
    int step = 1;
    if((mode == REVERSE) || (mode == ALNUM_REVERSE)){
        str1_stepper += str1->len;
        str2_stepper += str2->len;

        str1_beg += str1->len;
        str2_beg += str2->len;
        
        step = -1;
    }

    int alnum_mode = 0;
    
    if((mode == ALNUM_REVERSE) || (mode == ALNUM)){
        alnum_mode = 1;
    }

    // сравнение значений букв
    for(; (abs(str1_stepper - str1_beg) < str1->len) && (abs(str2_stepper - str2_beg) < str2->len); str2_stepper += step, str1_stepper += step){
        
        if(alnum_mode){
            while((abs(str1_stepper - str1_beg) < str1->len) && !isalnum_rus(*str1_stepper)){
                str1_stepper += step;
            }
            while((abs(str2_stepper - str2_beg) < str2->len) && !isalnum_rus(*str2_stepper)){
                str2_stepper += step;
            }
            if(!((abs(str1_stepper - str1_beg) < str1->len) && (abs(str2_stepper - str2_beg) < str2->len))){
                break;
            }
        }
        if(*str1_stepper != *str2_stepper){
            return (*str1_stepper) - (*str2_stepper);
        }
    }
    if((abs(str1_stepper - str1_beg) >= str1->len) && ((abs(str2_stepper - str2_beg) >= str2->len))){
        return 0;
    }
    else if((abs(str1_stepper - str1_beg) < str1->len)){
        return 1;
    }
    else{
        return -1;
    }
}

