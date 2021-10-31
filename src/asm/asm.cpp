#include "asm.h"

static char* init_code_array();

static void destruct_code_array(char* code_array);

static ERROR_CODES parse_label(char const * cmd_str, char labels[][MAX_LABEL_SIZE], int* ip);

static ERROR_CODES parse_cmd(_cmd_info *cmd_info, char const * str, char labels[][MAX_LABEL_SIZE], int* ip);

static int get_label_ip(char const *cmd_str, char labels[][MAX_LABEL_SIZE]);

static int is_jump(char const * str);

#define PREPROCCESOR_TO_STRING(name) #name

#define DEF_CMD(_name, num, args, code)										\
	if(strcmp(PREPROCCESOR_TO_STRING(_name), cmd_info.name) == 0){			\
																			\
		code_array[ip++] = (num) | cmd_info.arg.type;						\
																			\
		for(int n_arg = 0; n_arg < cmd_info.n_args; n_arg++){				\
			*(int*)(code_array + ip) = cmd_info.arg.value[n_arg];			\
			ip += ARG_SIZE;													\
		}																	\
	}																		\
	else

void Compile(char const * const asm_file_name, char const * const bin_file_name){

	assert(asm_file_name != NULL);
	assert(bin_file_name != NULL);
	assert(asm_file_name != bin_file_name);

	text_storage asm_text = {};

	if(get_text_storage(asm_file_name, &asm_text) != func_codes::OK){
		printf("error: file can't be readen\n");
		return;
	};

	char *code_array = init_code_array();

	char labels[MAX_INSTRUCTIONS_NUM][MAX_LABEL_SIZE] = {};

	int ip = 0;

	for(int stage = 0; stage < N_STAGES; stage++){

		ip    	  = N_SIGNATURES;
		int n_cmd = 0;

		for(unsigned int n_line = 0; n_line < asm_text.num_lines; n_line++){

			_cmd_info cmd_info = {};

			ERROR_CODES parse_res = parse_cmd(&cmd_info, asm_text.p_lines[n_line].pointer, labels, &ip);

			if(parse_res == ERROR_CODES::OK);
			else if((parse_res == ERROR_CODES::EMPTY_LINE) || (parse_res == ERROR_CODES::LABEL)){
				continue;
			}
			else if(parse_res == ERROR_CODES::WRONG_FORMAT){

				LOG_ERROR_MSG("invalid format");
				return;
			}
			else if(parse_res == ERROR_CODES::WRONG_REGISTER_NAME){

				LOG_ERROR_MSG("invalid register name");
				return;
			}
			else if((parse_res == ERROR_CODES::WRONG_LABEL_NAME) && (stage == 1)){

				LOG_ERROR_MSG("invalid label name");
				return;
			}
			#include "../cmd_definitions.h"
			/* else */{
				LOG_ERROR_MSG("invalid command name");
				return;
			}

			n_cmd++;
		}
	}

	clear_mem_storage(&asm_text);

	// фиктивная HLT в конце кода
	code_array[ip++] = 0;

	FILE* code_file = fopen(bin_file_name, "wb");
	fwrite(code_array, sizeof(char), ip, code_file);
	fclose(code_file);

	destruct_code_array(code_array);

	return;
}

#undef DEF_CMD

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

static ERROR_CODES parse_label(char const * cmd_str, char labels[][MAX_LABEL_SIZE], int* ip){

	assert(cmd_str    != NULL);
	assert(labels != NULL);

	if(strlen(cmd_str) == 0){
		return ERROR_CODES::EMPTY_LINE;
	}

	char label_name[MAX_LABEL_SIZE] = "";
	
	if(sscanf(cmd_str, "%s", label_name) == 1){
		
		int label_len = strlen(label_name);

		if(label_name[label_len - 1] == ':'){

			label_name[label_len - 1] = '\0';
			strcpy(labels[*ip], label_name);

			return ERROR_CODES::LABEL;
		}
		else{
			return ERROR_CODES::OK;
		}
	}
	return ERROR_CODES::EMPTY_LINE;
}

static ERROR_CODES parse_cmd(_cmd_info *cmd_info, char const * cmd_str, char labels[][MAX_LABEL_SIZE], int* ip){

	assert(cmd_info != NULL);
	assert(cmd_str != NULL);

	if(strlen(cmd_str) == 0){
		return ERROR_CODES::EMPTY_LINE;
	}

	// избавляемся от комментариев
	char *p_comment = strchr(cmd_str, ';');
	if(p_comment != NULL){
		*p_comment = '\0';
	}

	int immed_const_val  = 0;
	char reg_name        = 0;

	// позиция последнего считаного символа
	int str_pos = 0;
	cmd_info->n_args = 0;

	if(parse_label(cmd_str, labels, ip) == ERROR_CODES::LABEL){
		return ERROR_CODES::LABEL;
	}

	if(sscanf(cmd_str, " %[a-z]%n", cmd_info->name, &str_pos) != 1){
		return ERROR_CODES::WRONG_FORMAT;
	}

	cmd_str += str_pos;
	str_pos = 0;

	if(is_jump(cmd_info->name)){

		cmd_info->arg.value[cmd_info->n_args] = get_label_ip(cmd_str, labels);
		cmd_info->arg.type |= IMMED_CONST_POS;

		if(cmd_info->arg.value[cmd_info->n_args++] == -1){
			return ERROR_CODES::WRONG_LABEL_NAME;
		}
		return ERROR_CODES::OK;
	}
	else if(sscanf(cmd_str, " %d%n", &immed_const_val, &str_pos) == 1){
		cmd_info->arg.value[cmd_info->n_args++] = immed_const_val * ARG_PRECISION;
		cmd_info->arg.type |= IMMED_CONST_POS;
	}

	else if(sscanf(cmd_str, " %1[a-z]x%n", &reg_name, &str_pos) == 1){
		cmd_info->arg.value[cmd_info->n_args++] = reg_name - 'a';
		
		REGISTER_FORMAT_CHECK

		cmd_info->arg.type |= REG_POS;
	}

	else if(sscanf(cmd_str," [%d]%n", &immed_const_val, &str_pos) == 1){
		cmd_info->arg.value[cmd_info->n_args++] = immed_const_val;

		cmd_info->arg.type |= RAM_POS;
		cmd_info->arg.type |= IMMED_CONST_POS;
	}

	else if(sscanf(cmd_str, " [%1[a-z]x]%n", &reg_name, &str_pos) == 1){
		cmd_info->arg.value[cmd_info->n_args++] = reg_name - 'a';
		
		REGISTER_FORMAT_CHECK
		
		cmd_info->arg.type |= RAM_POS;
		cmd_info->arg.type |= REG_POS;
	}

	if(strlen(cmd_str + str_pos) != 0){
		return ERROR_CODES::WRONG_FORMAT;
	}

	return ERROR_CODES::OK;
}

static int get_label_ip(char const *cmd_str, char labels[][MAX_LABEL_SIZE]){

	assert(cmd_str != NULL);
	assert(labels  != NULL);

	char label_name[MAX_LABEL_SIZE];

	sscanf(cmd_str, " %s", label_name);

	for(int n_cmd = N_SIGNATURES; n_cmd < MAX_INSTRUCTIONS_NUM; n_cmd++){
		
		if(strcmp(labels[n_cmd], label_name) == 0){
			return n_cmd;
		}
	}

	return -1;
}

static int is_jump(char const * str){

	assert(str != NULL);

	for(int i = 0; i < N_JUMPS; i++){
		if(strcmp(str, jump_names[i]) == 0) return 1;
	}

	return 0;
}
