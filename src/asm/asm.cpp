
#include "asm.h"

static void destruct_code_array(char* code_array);

static char* init_code_array();

static int parse_cmd(_cmd_string *cmd, char const * str);

// TODO: добавить регистры и опер память, парсинг параметров
// TODO: optional [переменное количество параметров]

#define PREPROCCESOR_TO_STRING(name) #name

#define DEF_CMD(_name, num, args, code)							\
	if(strcmp(PREPROCCESOR_TO_STRING(_name)__ cmd_string.name) == 0){	\
																\
		code_array[ip++] = num | cmd_string.arg.type;			\
																\
		for(int _i_ = 0; _i_ < cmd_string.n_args; _i_++){										\
			*(int*)(code_array + ip) = cmd_string.arg.value;							\
			ip += ARG_SIZE;												\
		}														\
	}															\
	else

// TODO: добавить функцию для обработки строки, корректность ввода, убрать комментарии и тп
void Assembling(char const * const in_asm_name, char const * const out_asm_name){

	assert(in_asm_name != NULL);

	text_storage asm_content = {};
	get_text_storage(in_asm_name, &asm_content);

	// TODO: придумать лучшее имя
	char *code_array = init_code_array();

	int ip = N_SIGNATURES;

	for(int n_cmd = 0; n_cmd < asm_content.num_lines; n_cmd++){


		_cmd_string cmd_string = {};
		if(parse_cmd(&cmd_string, asm_content.p_lines[n_cmd].pointer) != 0){
			printf("(%s)Wrong cmd format:\n|  [%d]: %s\n", in_asm_name, n_cmd + 1, asm_content.p_lines[n_cmd].pointer);
			return;
		}

		#include "../cmd_definitions.h"
		/* else */{
			printf("[%d]: wrong format", n_cmd + 1);
			assert(0);
		}
	}

	clear_mem_storage(&asm_content);

	FILE* code_file = fopen(out_asm_name, "wb");
	fwrite(code_array, sizeof(char), ip, code_file);
	fclose(code_file);

	destruct_code_array(code_array);

	return;
}

static char* init_code_array(){

	char* code_array = (char*)calloc((MAX_ARGS_NUM + 1) * ARG_SIZE * MAX_INSTRUCTIONS_NUM + N_SIGNATURES, sizeof(char));
	
	assert(code_array != NULL);

	*(code_array + 0) = INVARIANT_SIGNATURE;	
	*(code_array + 1) = VERSION;

	// *(code_array + 2) = ANY_SPECIFIC_INFO

	return code_array;
}

static void destruct_code_array(char* code_array){

	assert(code_array != NULL);

	free(code_array);
}

static int parse_cmd(_cmd_string *cmd, char const * str){

	assert(cmd != NULL);
	assert(str != NULL);
	assert((char*)str != (char*)cmd);

	// избавляемся от комментариев
	char *p_comment = strchr(str, ';');
	if(p_comment != NULL){
		*p_comment = '\0';
	}

	//TODO: доделать обработку таких аргументов [ax + 5](сейчас работают только одиночные)
	char register_name[MAX_INSTR_LEN]    = "";
	char trash_string[MAX_INSTR_LEN]     = "";
	char trash_string2[MAX_INSTR_LEN]    = "";
	
	int arg_val = 0;

	if(sscanf(str, "%[A-Z] %s %s", cmd->name, trash_string2, trash_string) == 1);
	else if(sscanf(str, "%[A-Z] [%d] %s", cmd->name, &arg_val, trash_string) == 2){
		cmd->arg.value = arg_val;
		cmd->arg.type |= RAM_POS;
		cmd->arg.type |= IMMED_CONST_POS;
		cmd->n_args    = 1;
	}

	else if(sscanf(str, "%[A-Z] %d %s", cmd->name, &arg_val, trash_string) == 2){
		//printf("%d\n", cmd->arg.type);
		cmd->arg.value = arg_val;
		cmd->arg.type |= IMMED_CONST_POS;
		cmd->n_args    = 1;
	}
	else if(sscanf(str, "%[A-Z] [%cx] %s", cmd->name, &arg_val, trash_string) == 2){
		cmd->arg.value = arg_val - 'a';
		assert(cmd->arg.value < N_REGISTERS);

		cmd->arg.type |= RAM_POS;
		cmd->arg.type |= REG_POS;
		cmd->n_args    = 1;
	}
	else if(sscanf(str, "%[A-Z] %cx %s", cmd->name, &arg_val, trash_string) == 2){
		cmd->arg.value = arg_val - 'a';
		assert(cmd->arg.value < N_REGISTERS);

		cmd->arg.type |= REG_POS;
		cmd->n_args    = 1;
	}
	else{
		return -1;
	}
	
	return 0;
}
