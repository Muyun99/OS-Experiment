#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
char *home;
char *dir;
int mysys(char *command)
{
	if(command[0] == '\0')
	{
		printf("command not found!\n");
		return 127; //"command not found!"
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
int choose_fun(char *cmd)
{
	char argv[100];
	strcpy(argv,cmd);

	if(argv[0] == '\0')
		return 0;
	char *token = strtok(argv, " ");

	if(strcmp(token,"cd") == 0)
		return 1;
	else if(strcmp(token,"exit") == 0)
		return 2;
	else
		return 0;
}

int mycd(char *command)
{
	printf("debug2");
	char *argv[100];
	char *token;
	int count = 0;
	char cmd[sizeof(command) + 1];
	strcpy(cmd,command);
	token = strtok(cmd," ");
	while(token != NULL)
	{
		argv[count++] = token;
		token = strtok(cmd," ");
	}
	argv[count] = 0;

	printf("%s\n",argv[0]);
	printf("%s\n",argv[1]);
	if(strcmp(argv[1],"") == 0)
	{
		home = getenv("HOME");
		chdir(home);
		dir = getcwd(NULL,0);
	}
	int cdSucc = chdir(argv[1]);
	if(cdSucc == -1)
		printf("cd:No such path %s\n",argv[1]);
    return 0;
}

int main()
{
	home = getenv("HOME");
	char buff[100];
	while(1)
	{
		dir = getcwd(NULL,0);
		printf("[%s]> ",dir);
		gets(buff);

		int cmdStatus = choose_fun(buff);

		if(cmdStatus == 0)
			mysys(buff);
		else if(cmdStatus == 1)
		{
			char targetdir[256];
			sscanf(buff,"cd %s",targetdir);
			chdir(targetdir);
		}
		else if(cmdStatus == 2)
			exit(0);
	}
}
