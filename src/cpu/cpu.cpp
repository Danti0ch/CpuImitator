#include "cpu.h"

#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <math.h>

static void asm_code_constructor(asm_code* code_array, char const * const bin_file_name);

static void asm_code_destructor(asm_code* code_array);

static void cpu_constructor(cpu_t* cpu_storage);

static void cpu_destructor(cpu_t* cpu_storage);

static ARG_TYPE get_arg(char* p_code, cpu_t* cpu_storage);

static void set_to_mem(char* p_code, cpu_t* cpu_storage, ARG_TYPE arg);

static void change_pix_val(cpu_t* cpu_storage, int n_pix, int val);

static void show_graphic(cpu_t* cpu_storage);

static char get_pix_symb(cpu_t* cpu_storage, int x, int y);

#define DEF_CMD(name, num, args, code)                  \
case CMD_##name:                                        \
    code;                                               \
    cpu_storage.registers[PC] += 1 + (args) * ARG_SIZE; \
    break;

void Proccessing(char const * const bin_file_name){

    assert(bin_file_name != NULL);

    open_log_file(LOG_FILE_NAME);

    asm_code code_array = {};
    asm_code_constructor(&code_array, bin_file_name);

    cpu_t cpu_storage = {};
    cpu_constructor(&cpu_storage);

    if(code_array.p[0] != INVARIANT_SIGNATURE){
        printf("error: signature is invalid\n");
        exit(0);
    }
    if(code_array.p[1] != VERSION){
        printf("error: version is invalid\n");
        exit(0);
    }

    for(;;){
        switch((char)(code_array.p[cpu_storage.registers[PC]] & CMD_NUM_MASK)){
            #include "../cmd_definitions.h"
            default:
                printf("pc = %d, wrong cmd num\n", cpu_storage.registers[PC]);
                exit(0);
                break;
        }
    }

    asm_code_destructor(&code_array);
    cpu_destructor(&cpu_storage);
    close_log_file();
    return;
}

static void asm_code_constructor(asm_code* code_array, char const * const bin_file_name){
    
    assert(bin_file_name != NULL);

    FILE *code_file = fopen(bin_file_name, "rb");

    if(code_file == NULL){
        printf("error: opening %s file\n", bin_file_name);
        exit(0);
    }

    fseek(code_file, 0, SEEK_END);
    size_t file_size = ftell(code_file);
    fseek(code_file, 0, SEEK_SET);

    code_array->p = (char*)calloc(file_size, sizeof(char));
    if(code_array->p == NULL){
        printf("error: bin array allocation\n");
        exit(0);
    }

    code_array->len = file_size;

    if(fread(code_array->p, sizeof(char), file_size, code_file) != file_size){
        printf("error: bin array recording\n");
        exit(0);
    }
    fclose(code_file);

    return;
}

static void asm_code_destructor(asm_code* code_array){

    assert(code_array != NULL);

    free(code_array->p);
    code_array->p = (char*)POISON_POINTER;
    code_array->len = 0;

    return;
}

static void cpu_constructor(cpu_t* cpu_storage){

    assert(cpu_storage != NULL);

    if(StackConstructor(&(cpu_storage->stk), STACK_INIT_SIZE) != ERROR_CODE::OK){
        LOG_ERROR_MSG("stack construction");
    }
    if(StackConstructor(&(cpu_storage->call_stack), STACK_INIT_SIZE) != ERROR_CODE::OK){
        LOG_ERROR_MSG("stack construction");
    }

    cpu_storage->p_ram = (ARG_TYPE*)calloc(RAM_SIZE, sizeof(ARG_TYPE));
    if(cpu_storage->p_ram == NULL){
        LOG_ERROR_MSG("ram allocation")
    }

    cpu_storage->p_graphic_ram = (char*)cpu_storage->p_ram + GPU_RAM_POS;

    memset(cpu_storage->p_graphic_ram, 0x00, GPU_RAM_SIZE);
    
    cpu_storage->registers[PC] = N_SIGNATURES;

    return;
}

static void cpu_destructor(cpu_t* cpu_storage){

    assert(cpu_storage != NULL);

    if(StackDestructor(&(cpu_storage->stk)) != ERROR_CODE::OK){
        LOG_ERROR_MSG("stack destruction");
    }
    if(StackDestructor(&(cpu_storage->call_stack)) != ERROR_CODE::OK){
        LOG_ERROR_MSG("stack destruction");
    }

    free(cpu_storage->p_ram);

    cpu_storage->p_ram      = (ARG_TYPE*)POISON_POINTER;
    cpu_storage->p_graphic_ram = (char*)POISON_POINTER;

    return;
}

static ARG_TYPE get_arg(char* p_code, cpu_t* cpu_storage){
    
    assert(p_code != NULL);
    assert(cpu_storage != NULL);

    ARG_TYPE arg = 0;

    if(*p_code & IMMED_CONST_POS){
        arg = *(ARG_TYPE*)(p_code + 1);
    }
    else if(*p_code & REG_POS){
        arg = cpu_storage->registers[(int)*(p_code + 1)];
    }
    else{
        LOG_ERROR_MSG("wrong cmd type");
    }

    if(*p_code & RAM_POS){
        Sleep(RAM_CALLBACK_TIME);

        if( arg < 0 || arg > RAM_SIZE){
            LOG_ERROR_MSG("invalid ram position");
        }

        arg = cpu_storage->p_ram[arg];
    }

    return arg;
}

static void set_to_mem(char* p_code, cpu_t* cpu_storage, ARG_TYPE arg){
    
    assert(p_code != NULL);

    int ram_pos = 0;

    if(*p_code & IMMED_CONST_POS){
        ram_pos = *(int*)(p_code + 1);
    }
    else if(*p_code & REG_POS){
        ram_pos = cpu_storage->registers[(int)*(p_code + 1)];
    }
    else{
        LOG_ERROR_MSG("invalid arg type");
    }

    if(*p_code & RAM_POS){
        Sleep(RAM_CALLBACK_TIME);

        if( ram_pos < 0 || ram_pos > RAM_SIZE){
            LOG_ERROR_MSG("invalid ram position");
        }
        cpu_storage->p_ram[ram_pos] = arg;
    }
    else if(*p_code & REG_POS){
        cpu_storage->registers[(int)*(p_code + 1)] = arg;
    }
    else{
        LOG_ERROR_MSG("invalid arg type");
    }

    return;
}

static void change_pix_val(cpu_t* cpu_storage, int n_pix, int val){

    assert(cpu_storage != NULL);

    if(n_pix > GPU_RAM_SIZE * N_BITS_IN_BYTE || n_pix < 0){
        LOG_ERROR_MSG("coordinate values are invalid");
    }

    int byte_offset = n_pix / N_BITS_IN_BYTE;
    int bit_offset  = n_pix % N_BITS_IN_BYTE;

    if(val == 0){
        *(cpu_storage->p_graphic_ram + byte_offset) &= (char)~(1 << bit_offset);
    }
    else{
        *(cpu_storage->p_graphic_ram + byte_offset) |= (char)(1 << bit_offset);
    }

    return;
}

static void show_graphic(cpu_t* cpu_storage){

    assert(cpu_storage != NULL);
    
    for(int y = 0; y < WINDOW_SIDE; y++){
        for(int x = 0; x < WINDOW_SIDE; x++){

            char pix = get_pix_symb(cpu_storage, x, y);
            printf("%c%c", pix, pix);
        }
        printf("\n");
    }
    printf("\n_____________________________________________\n");
}

static char get_pix_symb(cpu_t* cpu_storage, int x, int y){

    assert(cpu_storage != NULL);
    assert((x < WINDOW_SIDE) && (y < WINDOW_SIDE));

    int byte_offset = (y * WINDOW_SIDE + x) / N_BITS_IN_BYTE;
    int bit_offset  = (y * WINDOW_SIDE + x) % N_BITS_IN_BYTE;

    char pix_val = *(cpu_storage->p_graphic_ram + byte_offset);

    pix_val &= (char)(1 << bit_offset);

    return pix_val ? pix_1 : pix_0;
}
