
#include "asm.h"

static void destruct_code_array(char* code_array);

static char* init_code_array();


static ERROR_CODES parse_label(char const * str, char labels[][MAX_LABEL_SIZE], int* ip);

static ERROR_CODES parse_cmd(_cmd_string *cmd, char const * str, char labels[][MAX_LABEL_SIZE], int* ip, int mode);

/*
static ERROR_CODES parse_jumps(char const * str, _cmd_string* cmd, char labels[][MAX_LABEL_SIZE]);
*/

static ERROR_CODES jmp_proccessing(char const *str, _cmd_string* cmd, char labels[][MAX_LABEL_SIZE]);

int IS_CMD_JUMP(char *str);

#define PREPROCCESOR_TO_STRING(name) #name

#define DEF_CMD(_name, num, args, code)										\
	if(strcmp(PREPROCCESOR_TO_STRING(_name), cmd_string.name) == 0){		\
																			\
		code_array[ip++] = num | cmd_string.arg.type;						\
																			\
		for(int n_arg = 0; n_arg < cmd_string.n_args; n_arg++){				\
			*(int*)(code_array + ip) = cmd_string.arg.value[n_arg];			\
			ip += ARG_SIZE;													\
		}																	\
	}																		\
	else

// TODO: POP [bx + 5] fix

void Compile(char const * const asm_file_name, char const * const bin_file_name){

	assert(asm_file_name != NULL);
	assert(bin_file_name != NULL);
	assert(asm_file_name != bin_file_name);

	text_storage asm_content = {};
	if(get_text_storage(asm_file_name, &asm_content) != func_codes::OK){
		printf("error: file reading\n");
	};

	char *code_array = init_code_array();

	char labels[MAX_INSTRUCTIONS_NUM][MAX_LABEL_SIZE] = {};

	int ip = N_SIGNATURES;

	int n_cmd = 0;

	// STAGE 1: PARSING CMDS
	for(int n_line = 0; n_line < asm_content.num_lines; n_line++){
		printf("%d\n", n_line);
		_cmd_string cmd_string = {};

		ERROR_CODES parse_res = parse_cmd(&cmd_string, asm_content.p_lines[n_line].pointer, labels, &ip, STAGE1);

		if(parse_res == ERROR_CODES::OK);
		else if(parse_res == ERROR_CODES::WRONG_FORMAT){
			printf("ERROR from file (%s): wrong format\n"
				   "\t[%d]: %s\n", 
				   asm_file_name, n_cmd + 1, asm_content.p_lines[n_cmd].pointer);

			destruct_code_array(code_array);
			return;
		}
		else if(parse_res == ERROR_CODES::WRONG_REGISTER_NAME){

			printf("ERROR from file (%s): register names must be ax - %cx formatted\n"
				   "\t[%d]: %s\n", 
				   asm_file_name, N_REGISTERS + 'a', n_cmd + 1, asm_content.p_lines[n_cmd].pointer);

			destruct_code_array(code_array);
			return;
		}
		else if(parse_res == ERROR_CODES::WRONG_JMP_ARG){

			printf("ERROR from file (%s): no label found for this jump\n"
				   "\t[%d]: %s\n", 
				   asm_file_name, n_cmd + 1, asm_content.p_lines[n_cmd].pointer);

			destruct_code_array(code_array);
			return;
		}
		else if((parse_res == ERROR_CODES::EMPTY_LINE) ||(parse_res == ERROR_CODES::LABEL) || (parse_res == ERROR_CODES::CMD_JUMP)){
			continue;
		}
		#include "../cmd_definitions.h"
		/* else */{
			printf("ERROR from file (%s): invalid command name\n"
				   "\t[%d]: %s\n", 
				   asm_file_name, n_cmd + 1, asm_content.p_lines[n_cmd].pointer);

			destruct_code_array(code_array);
			return;
		}
		meow
		n_cmd++;
	}

	n_cmd = 0;
	// STAGE 2: PARSING JUMPS
	ip = N_SIGNATURES;
	for(int n_line = 0; n_line < asm_content.num_lines; n_line++){
		
		_cmd_string cmd_string = {};

		ERROR_CODES parse_res = parse_cmd(&cmd_string, asm_content.p_lines[n_line].pointer, labels, &ip, STAGE2);

		if(parse_res == ERROR_CODES::OK);
		else if((parse_res == ERROR_CODES::EMPTY_LINE) ||(parse_res == ERROR_CODES::LABEL)){
			continue;
		}

		#include "../cmd_definitions.h"
		/* else */{
			printf("ERROR from file (%s): invalid command name\n"
				   "\t[%d]: %s\n", 
				   asm_file_name, n_cmd + 1, asm_content.p_lines[n_cmd].pointer);

			destruct_code_array(code_array);
			return;
		}

		n_cmd++;
	}
	meow
	clear_mem_storage(&asm_content);

	FILE* code_file = fopen(bin_file_name, "wb");
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

static ERROR_CODES parse_label(char const * str, char labels[][MAX_LABEL_SIZE], int* ip){

	assert(str    != NULL);
	assert(labels != NULL);

	if(strlen(str) == 0){
		return ERROR_CODES::EMPTY_LINE;
	}

	char label_name[MAX_LABEL_SIZE] = "";
	int str_pos = 0;

	if(sscanf(str, "%s%n", label_name, &str_pos) == 1){

		if(label_name[strlen(label_name) - 1] == ':'){
			strcpy(labels[*ip], label_name);

			return ERROR_CODES::LABEL;
		}
		else{
			return ERROR_CODES::OK;
		}
	}

	return ERROR_CODES::EMPTY_LINE;
}

static ERROR_CODES parse_cmd(_cmd_string *cmd, char const * str, char labels[][MAX_LABEL_SIZE], int* ip, int mode){

	assert(cmd != NULL);
	assert(str != NULL);

	if(strlen(str) == 0){
		return ERROR_CODES::EMPTY_LINE;
	}

	// избавляемся от комментариев
	char *p_comment = strchr(str, ';');
	if(p_comment != NULL){
		*p_comment = '\0';
	}

	int immed_const_val  = 0;
	char reg_name        = 0;
	
	int str_pos = 0;
	cmd->n_args = 0;

	// TODO: может стоять поделить это всё по функциям???

	//TODO: сделать фиктивную HLT в конце
	//TODO: пофиксить ввод даблов
	if(parse_label(str, labels, ip) == ERROR_CODES::LABEL){
		return ERROR_CODES::LABEL;
	}

	if(sscanf(str, " %[A-Z]%n", cmd->name, &str_pos) != 1){
		return ERROR_CODES::WRONG_FORMAT;
	}

	str += str_pos;

	str_pos = 0;

	// TODO: обработка одинаковых меток
	if(IS_CMD_JUMP(cmd->name) == 0){

		if(mode == STAGE1){
			cmd->arg.value[cmd->n_args++] = INVALID_CMD_NUM;

			return ERROR_CODES::OK;
		}
		else{
			return jmp_proccessing(str, cmd, labels);
		}
	}

	else if(sscanf(str, " %d%n", &immed_const_val, &str_pos) == 1){

		cmd->arg.value[cmd->n_args++] = immed_const_val * ARG_PRECISION;
		cmd->arg.type |= IMMED_CONST_POS;
	}

	else if(sscanf(str, " %1[a-z]x%n", &reg_name, &str_pos) == 1){
		cmd->arg.value[cmd->n_args++] = reg_name - 'a';
		
		REGISTER_FORMAT_CHECK

		cmd->arg.type |= REG_POS;
	}

	else if(sscanf(str," [%d]%n", &immed_const_val, &str_pos) == 1){

		cmd->arg.value[cmd->n_args++] = immed_const_val;

		cmd->arg.type |= RAM_POS;
		cmd->arg.type |= IMMED_CONST_POS;
	}

	else if(sscanf(str, " [%1[a-z]x]%n", &reg_name, &str_pos) == 1){

		cmd->arg.value[cmd->n_args++] = reg_name - 'a';
		
		REGISTER_FORMAT_CHECK
		
		cmd->arg.type |= RAM_POS;
		cmd->arg.type |= REG_POS;
	}

	else if(sscanf(str, " [%1[a-z]x + %d]%n", &reg_name, &immed_const_val, &str_pos) == 2){

		cmd->arg.value[cmd->n_args++] = reg_name - 'a';
		cmd->arg.value[cmd->n_args++] = immed_const_val;

		REGISTER_FORMAT_CHECK
		
		cmd->arg.type |= RAM_POS;
		cmd->arg.type |= REG_POS;
		cmd->arg.type |= IMMED_CONST_POS;
	}

	if(strlen(str + str_pos) != 0){
		return ERROR_CODES::WRONG_FORMAT;
	}

	return ERROR_CODES::OK;
}

static ERROR_CODES jmp_proccessing(char const *str, _cmd_string* cmd, char labels[][MAX_LABEL_SIZE]){

	assert(str     != NULL);
	assert(cmd     != NULL);
	assert(labels  != NULL);

	char jump_arg_name[MAX_LABEL_SIZE];

	sscanf(str, "%s", jump_arg_name);

	for(int n_cmd = 0; n_cmd < MAX_INSTRUCTIONS_NUM; n_cmd++){
		if(strcmp(labels[n_cmd], jump_arg_name) == 0){
			
			cmd->arg.value[cmd->n_args++] = n_cmd;

			cmd->arg.type |= IMMED_CONST_POS;

			return ERROR_CODES::OK;
		}
	}

	return ERROR_CODES::WRONG_JMP_ARG;
}

int IS_CMD_JUMP(char *str){
	
	assert(str != NULL);

	// TODO: FIX
	if(strcmp("JUMP", str) == 0 || 
	   strcmp("JA", str)   == 0 ||
	   strcmp("JB", str)   == 0 ||
	   strcmp("JE", str)   == 0 ||
	   strcmp("JAE", str)  == 0 ||
	   strcmp("JBE", str)  == 0 ||
	   strcmp("JNE", str)  == 0 ||
	   strcmp("CALL",str)  == 0){
		return 0;
	}
	return -1;
}
