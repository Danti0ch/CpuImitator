#include <stdio.h>
#include "cpu.h"

int main(){

	open_log_file(LOG_FILE_NAME);

	Proccessing(BIN_FILE_NAME);

	close_log_file();
	
	return 0;
}
