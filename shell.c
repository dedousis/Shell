/*******************************************************************
*author Andreas Dedousis csd3018@csd.uoc.gr
*
*******************************************************************/
#include "shell.h"
#define TRUE 1

/*TODO
command list 
set
unset

*/

void type_prompt(){
  printf("%s@cs345sh/%s/$ ",getlogin(),strrchr(getcwd(NULL,0), '/')+1);
}

void read_command()
{
	int size;
	int i=0;
	int len=0;
	int flag=0;
	int flag_command=0;
	int tok=0;
	int x=0;
	int counter=0;
	int c1=0;
	char* input=malloc(sizeof(char)*1024);
	char* input1=malloc(sizeof(char)*1024);
	
	char* command=malloc(sizeof(char)*1024);
	char** param=malloc(sizeof(char*)*1024);
	char** param1=malloc(sizeof(char*)*1024);
	
	char* var=malloc(sizeof(char)*1024);
	char* set_command=malloc(sizeof(char)*1024);
	char* trimmed_set_command=malloc(sizeof(char)*1024);
	char* set_arguments=malloc(sizeof(char)*1024);
	
	fgets(input,1024,stdin);
	size=strlen(input);

	if(input[size-1]=='\n')/*replace \n*/
	{
		input[size-1]='\0';
	}
	if(strcmp(input,"\0")!=0){
		input=strtok(input," \t\v\n"); //split the input and pass it ot an array
		while(input!=NULL){
			param[i]=input;
			i++;
			input=strtok(NULL," \t\v\n");
		}
		
		//---------------START PARSING---------------------
		
		if(strcmp(param[0],"exit")==0){
			exit(EXIT_SUCCESS);
		}else if(strcmp(param[0],"set")==0){
			if(param[1]!=NULL)
			{
				while(len<strlen(param[1])){
					if(param[1][len]=='=') // check if there is =
					{
						while(x<len)
						{
							var[x]=param[1][x];
							x++;
						}
						x++;
						while(x<strlen(param[1])){
							set_command[tok]=param[1][x];
							tok++;
							x++;
						}
					}
					len++;
				}
				len=0;
				tok=0;
				x=0;
				if(var[0]=='_') // check the var if is correct
				{
					printf("ERROR\n");
				}else
				{
					while(len<strlen(var))
					{
						if(isdigit(var[len])!=0 ||isalpha(var[len])){
							flag=1;
						}
						len++;
					}
				}
				if(param[2]==NULL){
					if(set_command[0]=='\"'&&set_command[strlen(set_command)-1]=='\"') 
					{
						tok=1;
						while(tok<strlen(set_command)){
							if(tok!=strlen(set_command)-1){
							trimmed_set_command[x]=set_command[tok];// remove "
							x++;
							}
							tok++;
						}
						flag_command=1;
					}else if(set_command[0]!='\"'&&set_command[strlen(set_command)-1]!='\"')
					{
						trimmed_set_command=set_command;
						flag_command=1;
					}else {
						printf("ERROR: WRONG COMMAND \n");
					}
				}else{
					len=2;
					x=0;
					if(set_command[0]!='\"'){
						printf("ERROR: no '\"' \n");
					}else{
						tok=1;
						while(tok<strlen(set_command)){//command 1without "
							trimmed_set_command[x]=set_command[tok];
							x++;
							tok++;
						}
						while(param[len]!=NULL){
						strcat(trimmed_set_command," ");
						strcat(trimmed_set_command,param[len]);
						len++;
						}
						
					}
					if(trimmed_set_command[strlen(trimmed_set_command)-1]=='\"'){
						trimmed_set_command[strlen(trimmed_set_command)-1]='\0';//delete the last"
						flag_command=1;
					}else {
						printf("ERROR: WRONG COMMAND \n");
					}
				}
			}
			if(flag==1&&flag_command==1){
			insert_(var,trimmed_set_command);
			}
		}else if(param[0][0]=='$'){

			len=1;
			while(len<strlen(command))
			{
				input1[tok]=command[len];
				tok++;
				len++;
			}
			input=get_(input1);
			if(input==NULL){
				printf("No previous declaration of %s\n",input1);
			}else{
				input=strtok(input," \t\v\n"); //split the input and pass it ot an array
				while(input!=NULL){
					param1[c1]=input;
					c1++;
					input=strtok(NULL," \t\v\n");
				}
				if(strcmp(param1[0],"&")!=0){
					exec_command(param1[0],param1,c1);
				}
				
			}
		}else if(strcmp(param[0],"echo")==0){
			if(param[1]!=NULL){
				if(param[1][0]=='$'){
					len=1;
					while(len<strlen(param[1])){ //delete $
						input1[tok]=param[1][len];
						tok++;
						len++;
					}
					input=get_(input1);
					if(input==NULL){
						printf("No previous declaration of %s\n",input1);
					}else{
						printf("\" %s \" \n",input);
					}
				}else{
					exec_command(param[0],param,i);
				}
			}
		}
		else if(strcmp(param[0],"unset")==0){
			if(param[1]!=NULL){
				delete_(param[1]);
			}
			
		}else if(strcmp(param[0],"printlvars")==0){
			print_variables();
		}else if(strcmp(command,"cd")==0){
			if(param[1]!=NULL){
			char* path=getcwd(NULL,0);
			char* directory_path;
			int o=1;
			strcat(path,"/");
			directory_path=strdup(path);
			strcat(directory_path,param[1]);
			o=chdir(directory_path);			
			}
		}else if(size!=1&&strcmp(param[0],"&")!=0){
			exec_command(param[0],param,i);
		}
	}
}
void exec_command(char* command,char** param,int i)
{
	char* path="/bin/";
	char* command_path=malloc(sizeof(char)*1024);;
	int background_pro;

	//-------------Check if commnand has & 
	
	if(strcmp(param[i-1],"&")==0){ // check for backgroudprocesses
			background_pro=1;
			param[i-1]=NULL;
			i=i-1;
	}	
	command_path=strdup(param[0]);	
	int cell=0;
	int x=0;
	char* first_comm=malloc(sizeof(char)*1024);
	char* sec_comm=malloc(sizeof(char)*1024);
	char* third_comm=malloc(sizeof(char)*1024);
	char** first_comm_tab=malloc(sizeof(char*)*1024);
	char** sec_comm_tab=malloc(sizeof(char*)*1024);
	int bar_pipe=0;
	int input_from_file=0;
	int output_to_file=0;
	int append_to_file=0;
	int input_output_to_file=0;
	if(param[1]!=NULL)
	{
		while(param[cell]!=NULL)
		{
			if(strcmp(param[cell],"|")==0){ //pipe
				bar_pipe=1;
				while(x<cell)
				{
					strcat(first_comm,param[x]);
					strcat(first_comm," ");
					x++;
				}
				cell++;
				while(param[cell]!=NULL){
					strcat(sec_comm,param[cell]);
					strcat(sec_comm," ");
					cell++;
				}
			}
			else if(strcmp(param[cell],"<")==0) //input
			{
				input_from_file=1;
				while(x<cell)
				{
					strcat(first_comm,param[x]);
					strcat(first_comm," ");
					x++;
				}
				cell++;
				if(param[3]!=NULL){
					if(strcmp(param[3],">")==0){//check for output
						if(param[4]!=NULL){
						third_comm=strdup(param[4]);
						input_output_to_file=1;
						}
					}
				}
				while(param[cell]!=NULL){
					strcat(sec_comm,param[cell]);
					cell++;
				
				}
			}
			else if(strcmp(param[cell],">")==0) //output
			{
				output_to_file=1;
				while(x<cell)
				{
					strcat(first_comm,param[x]);
					strcat(first_comm," ");
					x++;
				}
				cell++;
				while(param[cell]!=NULL){
					strcat(sec_comm,param[cell]);
					cell++;
				}	
			}
			else if(strcmp(param[cell],">>")==0) //append
			{
				append_to_file=1;
				while(x<cell)
				{
					strcat(first_comm,param[x]);
					strcat(first_comm," ");
					x++;
				}
				cell++;
				while(param[cell]!=NULL){
					strcat(sec_comm,param[cell]);
					cell++;
				}	
			}
			cell++;
		}
		x=0;
		cell=0;
		first_comm=strtok(first_comm," \t\v\n"); //split the input and pass it ot an array
		while(first_comm!=NULL){
			first_comm_tab[x]=first_comm;
			x++;
			first_comm=strtok(NULL," \t\v\n");
		}
		if(bar_pipe==1){
			sec_comm=strtok(sec_comm," \t\v\n"); //split the input and pass it ot an array
			while(sec_comm!=NULL){
				sec_comm_tab[cell]=sec_comm;
				cell++;
				sec_comm=strtok(NULL," \t\v\n");
			}
		}
	}
	
	//--------------------START NEW PROCESS--------------------
	int pipefd[2];
	pid_t cpid;
	char buf;
	pid_t pid=fork();//child pid
	if(pid==0){
		
		//---------------------------CHILD---------------------
		if(background_pro==1)
		{
			execve(command_path,param,0);	//BAckground
			exit(EXIT_SUCCESS);
		}else{
			
			//-----------------------PIPE----------------------
			if(bar_pipe==1)
			{
				if(pipe(pipefd)!=0){
					printf("Error failed pipe\n");
					exit(EXIT_FAILURE);
				}
				if((cpid=fork())==-1){
					printf("Error in fork \n");
					exit(EXIT_FAILURE);
				}else if(cpid==0) //-------new child process read from pipe
				{
					dup2(pipefd[1],1);
					close(pipefd[0]);
					close(pipefd[1]);
					execvp(first_comm_tab[0],first_comm_tab);
					exit(EXIT_SUCCESS);
				}else{ 						// ---------shell write to  pipe
					dup2(pipefd[0],0);
					close(pipefd[0]);
					close(pipefd[1]);
					wait(NULL);
					execvp(sec_comm_tab[0],sec_comm_tab);
					exit(EXIT_SUCCESS);
				}
			}
			
			//-----------------------REDIRECTION---------------------
			if(input_from_file==1&&input_output_to_file==0)
			{
				int in;
				in=open(sec_comm,O_RDONLY);
				dup2(in,STDIN_FILENO);
				close(in);
				command_path=first_comm_tab[0];
				param=first_comm_tab;
			}
			if(output_to_file==1)
			{
				int out;
				out=open(sec_comm,O_WRONLY | O_CREAT,0666);
				dup2(out,STDOUT_FILENO);
				close(out);	
				command_path=first_comm_tab[0];
				param=first_comm_tab;
			}
			if(append_to_file==1)
			{
				int out;
				out=open(sec_comm,O_WRONLY|O_CREAT|O_APPEND);
				dup2(out,STDOUT_FILENO);
				close(out);
				command_path=first_comm_tab[0];
				param=first_comm_tab;
			}
			if(input_output_to_file==1)
			{

				int in;
				int out;
				in = open(param[2],O_RDONLY);
				dup2(in,STDIN_FILENO);
				close(in);
				out=open(third_comm,O_WRONLY | O_CREAT,0666);
				dup2(out,STDOUT_FILENO);
				close(out);
				command_path=first_comm_tab[0];
				param=first_comm_tab;
			}
			
			//-----------------EXECUTION----------
			execvp(command_path,param);
			exit(EXIT_SUCCESS);
		}
	}else if(pid>0)
	{
		//PARENT
		if(background_pro==1)
		{
			//Father doesnt wait for child process 
			printf("Background process:\nCommand: %s %d\n\n",command,pid);
		}else{
			waitpid(pid,NULL,0); //wait for the children to exit if not background
		}	
	}else if(pid<0)
	{
		//ERROR
		printf("Error in fork \n");
		exit(EXIT_FAILURE);
	}
}

int insert_(char* var,char* command)
{
	struct set_variable* node=(struct set_variable *)malloc(sizeof(struct set_variable));
	node->var=strdup(var);
	node->command=strdup(command);
	if(head==NULL){
		head=node;
		head->next=NULL;
	}else{
			node->next=head;
			head=node;
	}
	return 1;
}

char* get_(char* var)
{
	struct set_variable* node=head;
	if(var!=NULL&&head!=NULL){
		while(node!=NULL)
		{
			if(strcmp(node->var,var)==0){
				return node->command;
			}
			node=node->next;
		}
	}
	return NULL;
}

int delete_(char* var)
{
	struct set_variable* node=head;
	struct set_variable* prev;
	if(var!=NULL&&head!=NULL){
		//prev=head;
		while(node!=NULL)
		{
			if(strcmp(node->var,var)==0){
				if(node==head){
					head=node->next;
					free(node);
					return 1;
				}else{
					prev->next=node->next;
					node->next=NULL;
					free(node);
					return 1;
				}
			}
			prev=node;
			node=node->next;
		}
	}
	return 0;
}
void print_variables()
{
	struct set_variable* node;
	int x=0;
	if(head==NULL){
		printf("Error: no vars declared\n");
	}else{
		node=head;
		while(node!=NULL)
		{
			printf("Variable : %s = ",node->var);
			printf("%s",node->command);
			printf("\n");
			node=node->next;
		}
	}
}
int main()
{
	while(TRUE){
		type_prompt();
		read_command();	
	}
return 0;
}
