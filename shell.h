/*******************************************************************
*author Andreas Dedousis csd3018@csd.uoc.gr
*
*******************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
 struct set_variable{
	char* var;
	char* command;
	struct set_variable* next;
};

struct set_variable* head=NULL;

void type_prompt();
void read_command();
void exec_command(char* command,char** param,int i);

int insert_(char* var,char* command);
int delete_(char* var);
char* get_(char* var);
void print_variables();