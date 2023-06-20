#include "disasm.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../cmd_constants.h"
#include "log.h"

static void asm_code_constructor(asm_code* code_array, char const * const bin_file_name);

static void asm_code_destructor(asm_code* code_array);

void print_cmd(char* p_cmd, const char* cmd_name, int pos, int n_arg);

#define DEF_CMD(name, num, args, code)                                          \
case CMD_##name:                                                                \
    print_cmd(code_array.p + ip, (#name), ip, (args));                          \
    ip += 1 + (args) * ARG_SIZE;                                                \
    break;

void Disassembling(char const * const bin_file_name, char const * const log_file_name){

    assert(bin_file_name != NULL);
    assert(log_file_name != NULL);

    asm_code code_array = {};
    asm_code_constructor(&code_array, bin_file_name);

    if(code_array.p[0] != INVARIANT_SIGNATURE){

        printf("error: signature is invalid\n");
        asm_code_destructor(&code_array);
        exit(0);
    }

    if(code_array.p[1] != VERSION){

        printf("error: version is invalid\n");
        asm_code_destructor(&code_array);
        exit(0);
    }

    open_log_file(log_file_name);

    int ip = N_SIGNATURES;
    while(ip < (int)code_array.len){

        switch((char)(code_array.p[ip] & CMD_NUM_MASK)){

            #include "../cmd_definitions.h"
            default:
                to_log("ERROR from file (%s): invalid format\n"
                       "\tip = [%d]\n", 
                       bin_file_name, ip);
                break;
        }
    }

    asm_code_destructor(&code_array);
    close_log_file();
    
    return;
}

#undef DEF_CMD

static void asm_code_constructor(asm_code* code_array, char const * const bin_file_name){
    
    assert(bin_file_name != NULL);

    FILE *code_file = fopen(bin_file_name, "rb");

    assert(code_file != NULL);
    
    fseek(code_file, 0, SEEK_END);
    size_t file_size = ftell(code_file);
    fseek(code_file, 0, SEEK_SET);

    code_array->p = (char*)calloc(file_size, sizeof(char));
    assert(code_array->p != NULL);

    code_array->len = file_size;

    assert(fread(code_array->p, sizeof(char), file_size, code_file) == file_size);
    fclose(code_file);

    return;
}

static void asm_code_destructor(asm_code* code_array){

    assert(code_array != NULL);

    free(code_array->p);

    code_array->len = 0;

    return;
}

void print_cmd(char* p_cmd, const char* cmd_name, int pos, int n_arg){
    
    assert(p_cmd != NULL);

    to_log("%04X | ", pos);
    to_log("%02X ", *p_cmd & CMD_NUM_MASK);

    if(n_arg == 1){

        int arg_val = *(int*)(p_cmd + 1);
        to_log("%08X ", arg_val);
    }
    else{
        to_log("%*c ", ARG_SIZE * 2, ' ');
    }

    to_log("| %s ", cmd_name);
    
    if(n_arg == 1){
        to_log("%d", *(int*)(p_cmd + 1));
    }
    
    to_log("\n");

    return;
}
