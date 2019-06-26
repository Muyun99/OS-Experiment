#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
char *home;
char *dir;
int std_in;
int std_out;
int mysys(char *command)
{
	int status = -1;
	if(command[0] == '\0')
	{
		printf("command not found!\n");
		return 127; //"command not found!"
	}
    pid_t pid;
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
	    if(execvp(argv[0],argv)==-1)
		{
			printf("exec failed: ");
			printf("%d\n",errno);
		}
		else
			status = 1;
    }
    else
        while(waitpid(pid,NULL,0) < 0)
		{
			if(errno!=EINTR)
				status = -1;
				break;	
		}
	
	dup2(std_in,0);
	dup2(std_out,1);
	return status;
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

int loop()
{
	printf("debug1");
	char buff[100];
	char tempstr[100];
	home = getenv("HOME");
	dir = getcwd(NULL,0);
	printf("[%s]> ",dir);
	gets(buff);

	char *a = NULL;
	char *b = NULL;
	a = strchr(buff, '<');
	b = strchr(buff, '>');
	int inindex = 0;
	int outindex = 0;
	int count = 0;

	char *argv[100];
	char *token;
	char cmd[sizeof(buff) + 1];
	strcpy(cmd, buff);
	token = strtok(cmd,"");
	while(token != NULL)
	{
		if(strchr(token,'<'))
			inindex = count+1;
		else if(strchr(token,'>'))
			outindex = count+1;
		argv[count++] = token;
		token = strtok(cmd," ");
	}

	if(a != NULL && b != NULL)
	{
		char *in = argv[inindex];
		char *out = argv[outindex];
		int fdin,fdout;
		fdin = open(in,O_RDWR,0666);
		fdout = open(out,O_CREAT|O_RDWR,0666);
		if(fdin == -1)
		{
			printf("File %s open failed!\n",in);
			return -1;
		}
		if(fdout == -1)
		{
			printf("File %s open failed!\n",out);
			return -1;
		}
		dup2(fdin,0);
		dup2(fdout,1);
		close(fdin);
		close(fdout);
		return mysys(buff);
	}
	else if(a != NULL)
	{
		char *in = argv[inindex];
		int fdin = open(in, O_RDWR, 0666);
		dup2(fdin,0);
		close(fdin);
		return mysys(buff);
	}
	else if(b != NULL)
	{
		char *out = argv[outindex];
		int fdout = open(out, O_CREAT|O_RDWR, 0666);
		dup2(fdout,1);
		close(fdout);
		return mysys(buff);
	}
	else
	{
		printf("debug1");
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
int main()
{
	std_in = dup(0);
	std_out = dup(1);
	while(1)
	{
		loop();
	}
	return 0;
}
