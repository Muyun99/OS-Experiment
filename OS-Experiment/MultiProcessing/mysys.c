#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int mysys(char *command)
{
	if(command[0] == '\0')
	{
		printf("command not found!\n");
		return 127; // "command not found!"
	}
    int pid;
    pid = fork();
    if(pid == 0)
    {
        char *argv[100];
        char *token;
        char cmd[sizeof(command) + 1];
        strcpy(cmd, command);
    
		//get first substr
        token = strtok(cmd, " ");
        int count = 0;
        while(token != NULL)
        {
            argv[count++] = token;
            token = strtok(NULL," "); 
        }
        argv[count] = 0;
        execvp(argv[0],argv);
    }
    else
        wait(NULL);
}

int main()
{
	mysys("");
    printf("-------------------------------------------\n");
	mysys("pwd");

	printf("-------------------------------------------\n");
	mysys("ls");

    printf("-------------------------------------------\n");
    mysys("echo HELLO WORLD");
    
    printf("-------------------------------------------\n");
    mysys("ls /");

    printf("-------------------------------------------\n");
    return 0;
}
