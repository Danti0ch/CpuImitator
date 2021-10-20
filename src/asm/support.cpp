/**
 * \file
 * 
 * \brief файл содержит определения функций, обьявленных в support.h
 */

#include "support.h"

void show_help(void){

	FILE *helper_file = fopen("readme.txt", "r");

	if(helper_file == NULL){
		printf("file \"readme.txt\" was not found in the current directory\n"
			   "||\tbrief description of the project\t||\n"
               "the project implements functions for working with groups of strings:\n"
			   "sorting strings, getting strings from a file and writing strings to a file\n");
		return;
	}

    display_file_content(helper_file);
}

enum func_codes swap(void *p_elem1, void *p_elem2, const size_t size_elem){

   assert(p_elem1 != NULL);
   assert(p_elem2 != NULL);

   assert(size_elem > 0);

   if(p_elem1 == p_elem2){
      return IDENTICAL_POINTERS;
   }

   long long int big_storage = 0;

   long long int *bstepper_elem1 = (long long int *)p_elem1;
   long long int *bstepper_elem2 = (long long int *)p_elem2;

   // элемент размера sizeof long long int должен помещаться в оставшееся место p_elem
   for(; (get_bytes(p_elem1, bstepper_elem1 + 1) <= size_elem) && (get_bytes(p_elem2, bstepper_elem2 + 1) <= size_elem); bstepper_elem1++, bstepper_elem2++){
      big_storage = *bstepper_elem2;
      *bstepper_elem2 = *bstepper_elem1;
      *bstepper_elem1 = big_storage;
   }

   // разные размеры переменных
   assert(!((get_bytes(p_elem1, bstepper_elem1 + 1) <= size_elem) && (get_bytes(p_elem2, bstepper_elem2 + 1) <= size_elem)));

   char small_storage = 0;

   char *sstepper_elem1 = (char *)bstepper_elem1;
   char *sstepper_elem2 = (char *)bstepper_elem2;
   
   // записанная часть p_elem не должна превосходить размер элемента
   for(; (get_bytes(p_elem1, sstepper_elem1 + 1) <= size_elem) && (get_bytes(p_elem2, sstepper_elem2 + 1) <= size_elem); sstepper_elem1++, sstepper_elem2++){
      small_storage = *sstepper_elem2;
      *sstepper_elem2 = *sstepper_elem1;
      *sstepper_elem1 = small_storage;
   }

   //разные размеры переменных
   assert(!((get_bytes(p_elem1, sstepper_elem1 + 1) <= size_elem) && (get_bytes(p_elem2, sstepper_elem2 + 1) <= size_elem)));

   return OK;
}

// x6,5 faster than fgetc stuff
enum func_codes get_file_len(const char *file_name, size_t *n_lines, size_t *len){

    assert(file_name != NULL);

    assert(n_lines != NULL);
    assert(len != NULL);
    assert(len != n_lines);
    
    *len = 0;
    *n_lines = 0;

    FILE *text_file = fopen(file_name, "r");

    assert(text_file != NULL);
    
    fseek(text_file, 0, SEEK_END);

    size_t file_size = ftell(text_file);

    if(file_size <= 0){
        fclose(text_file);
        return EMPTY_FILE;
    }

    fseek(text_file, 0, SEEK_SET);

    char *buffer = (char*)calloc(file_size + 1, sizeof(char));

    size_t n_readen_bytes = fread(buffer, sizeof buffer[0], file_size + 1, text_file);

    assert(n_readen_bytes > 0);

    *len = n_readen_bytes;

    for(size_t n_byte = 0; n_byte < n_readen_bytes; n_byte++){
        if(buffer[n_byte] == '\n'){
            (*n_lines)++;
        }
    }

    // если на последней непустой строчке нет символа переноса
    if((buffer[n_readen_bytes - 1] > 0) && (buffer[n_readen_bytes - 1] != '\n')){
        (*n_lines)++;
    }

    free(buffer);
    fclose(text_file);

    return OK;
}

enum func_codes display_file_content(FILE *input_file){

    assert(input_file != NULL);

    char line[MAX_README_LINE_LEN] = "";
    while(fgets(line, MAX_README_LINE_LEN, input_file) != NULL){
        printf("%s", line);
    }

    fseek(input_file, 0, SEEK_SET);

    return OK;
}

inline size_t get_bytes(void *left_pointer, void *right_pointer){

    return (char*)(right_pointer) - (char *)(left_pointer);
}

int isalnum_rus(int c){
    
    return (isalnum(c) || ((c >= -64) && (c < 0)));
}

int get_console_parms(const int argc, char const * const argv[], char* file_name, int *is_alnum_sorting){
    
    assert(argc > 0);
    assert(argv != NULL);

    int argc_current = 1;

    *is_alnum_sorting = 0;

    if((argc == 2) && (strcmp("--help", argv[argc_current]) == 0)){
        show_help();
        return 0;
    }

    if((argc > 1) && (argv[argc_current][0] != '-') && (strlen(argv[argc_current]) < MAX_INPUT_FILE_NAME_LEN)){
        strcpy(file_name, argv[argc_current]);
        argc_current++;
    }

    // checking the correction of file
    FILE *input_file = fopen(file_name, "r");
    if(input_file == NULL){
        fprintf(stderr, "No file found \"%s\"\n", file_name);
        return 0;
    }
    fclose(input_file);

    for(; argc_current < argc; argc_current++){
        if(strcmp("--alnum", argv[argc_current]) == 0){
            *is_alnum_sorting = 1;
        }
        else{
            fprintf(stderr, "wrong flag \"%s\"\n", argv[argc_current]);
            return 0;
        }
    }

    printf("Не без кота.\n");

    return 1;
}