#include "./exalt_command.h"


/**
 * @brief execute a command
 * @return Return the process pointer
 */
FILE* exalt_execute_command(char* cmd)
{
	FILE *p;

	p = popen(cmd, "r");
	if(p==NULL)
	{
		printf("execute_command(): popen failed: \n");
		return NULL;
	}

	return p;
}



