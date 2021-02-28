#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include"linkedlist.h"
#include"history.h"
#include<signal.h>
 
int shell_cd(char** args)
{
	if(args[1]==NULL)	fprintf(stderr,"shell: expected argument to \"cd\"\n");
	else
	{
		if(chdir(args[1])!=0)  perror("Invalid Call");
	}
	return 1;
}

int shell_exit(char **args)
{
	return 0;
}

process* headProcess=NULL;
int shell_bg(char **args)
{
//args -- bg echo "hello"
    ++args;
//args -- echo "hello"
    char *firstCmd= args[0];//echo
    int childpid=fork();
    if (childpid>=0)
    {
        if (childpid==0)
        {
            if (execvp(firstCmd,args)<0)
            {
                perror("Error on execvp\n");
                exit(0);
            }

        }
        else{
                if (headProcess==NULL)    headProcess=create_list(childpid,firstCmd);
                 else  add_to_list(childpid,firstCmd,true);
        }
    }
    else perror("fork() error");
    return 1;
}
int shell_bglist(char **args)
{
    print_list();
    return 1;
}
int shell_kill(char **args)
{
    // kill 1575
    char* pidCmd=args[1];
    if(!pidCmd)    printf("Please specify a pid\n");
    
    else
    {
        int pid=atoi(pidCmd);
        process* target= search_in_list(pid, &headProcess);
        if (target!=NULL)
        {
            if (kill(target->pid,SIGTERM)>=0)	delete_from_list(pid);            
            else	perror("Could not kill pid specified\n");
            
        }
        else	printf("Specify a pid which is present in the list.\nType \"bglist\" to see active processes\n");
        
    }
    return 1;
}
char* builtin_str[]={"cd","exit","bg","bglist","kill"};
//bg echo "hello"
//bglist ->jobs
//kill 
int (*builtin_func[]) (char**)={&shell_cd,&shell_exit,&shell_bg,&shell_bglist,&shell_kill};



char *shell_read_line()
{
	char*line=NULL;
	ssize_t bufsize =0;
	if(getline(&line,&bufsize,stdin)==-1)
	{
		if(feof(stdin))  exit(EXIT_SUCCESS);
		else
		{
			perror("acm-shell: getline\n");
			exit(EXIT_FAILURE);
		}
	}
	return line;
} 

#define SHELL_TOKEN_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n"

char** shell_split_line(char* line)
{
	int bufsize=SHELL_TOKEN_BUFSIZE,position=0;        
	char ** tokens=malloc(bufsize * sizeof(char*));   
	char *token; 

	if(!token)            // doubt
	{
		fprintf(stderr,"acm-shell: allocation error\n");     
		exit(EXIT_FAILURE);
	}

	//strtok :- Library Function 
	token=strtok(line,SHELL_TOK_DELIM); // cd a 
	while(token!=NULL)
	{
		tokens[position]=token;
		token=strtok(NULL,SHELL_TOK_DELIM);

		position++;
	}
	tokens[position]=NULL;
	return tokens;
}

int shell_launch(char **args)
{
	pid_t pid;
	int status;

	pid=fork();
	if(pid==0)
	{
		if(execvp(args[0],args)==-1)	printf("Invalid_Command\n");
		exit(EXIT_FAILURE);
	}
	else if(pid<0)   perror("acm-shell");
	
	else
	{
		do
		{
			waitpid(pid,&status,WUNTRACED);
		}
		while(!WIFEXITED(status)&&!WIFSIGNALED(status));
	}
	return 1;
}

int shell_execute(char ** args)
{
	int i;
	if(args[0]==NULL)  return 1;

	for(int i=0;i<5;i++) 
	{
		if(strcmp(args[0],builtin_str[i])==0)  // false 0	if both strings are identical (equal)
			return (*builtin_func[i])(args);       //....
	}

	return shell_launch(args);

}

void broadcastTermination(int pid,int status){
    if (WIFEXITED(status)){  //wif... returns something according to how signal was handled
        printf("exited, status=%d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)){
        printf("killed_by_signal %d\n", WTERMSIG(status));
    }
    else if(WIFSTOPPED(status))
    {
        printf("stopped_by_signal %d\n", WSTOPSIG(status));
    }
    else if(WIFCONTINUED(status))
    {
        printf("continued\n");
    }
    delete_from_list(pid); // where is the list
}

static void signalHandler(int sig){
    int pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);  // pid_t waitpid(pid_t pid, int *status ((LIKE NULL)), int options((how to wait))); 
									  //-1: Wait for any child process., WNOHANG: Return immediately if no child has exited.
    broadcastTermination(pid,status); //
}


int main(int argc,char **argv)  //will take command ls -a type 
{
	char *line;    //string
	char **args;	//array of string


	// where is child process ?
	int status; 

	//sent a signal 
    signal(SIGCHLD,signalHandler);               
	do{
		
		printf("PRAVAL-SHELL > ");
		line=shell_read_line();
		args=shell_split_line(line);
		status=shell_execute(args);
		

		free(line);
		free(args);
	}
	while(status);

	return 0;
}
