#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_BUFFLEN	1024
#define MAX_NUM 100

char *home;
char *dir;
int recover_in;
int recover_out;

void split(char *src, int *argc, char **argv)
{
	char code[MAX_BUFFLEN];
    int count = 0;      // N.O. of arguments
	char *next = NULL;
	char *rest = code;
	strcpy(code, src);
	argv[count++] = code;
    while(next = strchr(rest, ' '))
    {
        next[0] = '\0';
        rest = next + 1;
        // printf("rest = \"%s\"\n", rest);
        
        if(rest[0] != '\0' && rest[0] != ' ')
            argv[count++] = rest;
        if(count + 2 > MAX_NUM)
            return ;
    }
    argv[count++] = NULL;
	*argc = count - 1;
}

int mysys(const char *cmdstring)
{
	pid_t pid;  
    int status = -1;  
  
    if (cmdstring == NULL)  
        return 1;  
      
    if ((pid = fork()) < 0)  
        status = -1;  
    else if (pid == 0)  
    {  
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);  
        exit(127);  
    }  
    else  
    {  
        while (waitpid(pid, &status, 0) < 0)  
        {  
            if (errno != EINTR)  
            {  
               status = -1;  
               break;  
            }  
        }  
    }  
	
	//恢复重定向
	dup2(recover_in, 0);
	dup2(recover_out, 1);
    return status;  
}

int judge_buff(char *buff)
{
	if(buff[0] == '\0')
		return 0;
	char code[MAX_BUFFLEN];
	strcpy(code, buff);
	char *next = strchr(code, ' ');
	if(next != NULL)
		next[0] = '\0';
	//printf("[code] %s", code);
	if(strcmp(code, "cd") == 0)
		return 1;
	else if(strcmp(code, "exit") == 0)
		exit(0);
	else 
		return 0;
}

int cd(char *buff)
{
	int argc = 0;
    char *argv[MAX_NUM];    // no more than 100 arguments
    int count = 0;      // N.O. of arguments
	split(buff, &argc, argv);
	count = argc;
	
	if(count == 1)
	{
		chdir(home);
		dir = getcwd(NULL, 0);
	}
	else
	{
		int res = chdir(argv[count - 1]);
		dir = getcwd(NULL, 0);
		if(res == -1)
		{
			printf("cd: No such path %s\n", argv[count - 1]);
			return -1;
		}
	}
	return 0;
}

int go(char *buff)
{
	int res = judge_buff(buff);
	if(res == 0)
		mysys(buff);
	else if(res == 1)
		cd(buff);
	else if(res == 2)
		return 2;
	return 1;
}

void strip_char(char *s, char bad)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    for(i = 0; i < len; ++i){
        char c = s[i];
        if(c==bad) ++offset;
        else s[i-offset] = c;
    }
    s[len-offset] = '\0';
}

void strip_dup(char *s)
{
    size_t i;
    size_t len = strlen(s);

    for(i = 0; i < len; ++i)
	{
        char c = s[i];
        if(c == '<' || c == '>')
			s[i] = '\0';
    }
}

int go_dup(char *buff)
{
	char code[MAX_BUFFLEN];
	strcpy(code, buff);

	char *a = NULL;
	char *b = NULL;
	a = strchr(buff, '<');
	b = strchr(buff, '>');
	
	strip_dup(code);
	if(a != NULL && b != NULL)
	{
		char *in = a + 1 - buff + code;
		char *out = b + 1 - buff + code;
		strip_char(in, ' ');
		strip_char(out, ' ');
		// printf("[in] %s\n", in);
		// printf("[out]%s\n", out);
		// printf("[code]%s\n", code);
		int fdin, fdout;
		fdin = open(in, O_RDWR, 0666);
		fdout = open(out, O_CREAT|O_RDWR, 0666);
		if(fdin == -1)
		{
			printf("File %s open faild\n", in);
			return -1;
		}
		if(fdout == -1)
		{
			printf("File %s open faild\n", out);
			return -1;
		}
		dup2(fdin, 0);
		dup2(fdout, 1);
		close(fdin);
		close(fdout);
		return mysys(code);
	}
	else if(a != NULL)
	{
		char *in = a + 1 - buff + code;
		strip_char(in, ' ');
		int fdin;
		fdin = open(in, O_RDWR, 0666);
		dup2(fdin, 0);
		close(fdin);
		return mysys(code);
	}
	else if(b != NULL)
	{
		char *out = b + 1 - buff + code;
		strip_char(out, ' ');
		int fdout;
		fdout = open(out, O_CREAT|O_RDWR, 0666);
		dup2(fdout, 1);
		close(fdout);
		return mysys(code);
	}
	else
	{
		return go(buff);
	}

}

void print_prefix()
{
	if(strcmp(home, dir) == 0)
		printf("[~]$ ");
	else
		printf("[%s]$ ", dir);
}

int main()
{
	recover_in = dup(0);
	recover_out = dup(1);

	home = getenv("HOME");
	dir = getcwd(NULL, 0);
	char buff[MAX_BUFFLEN];

	print_prefix();
	while(gets(buff))
	{
		go_dup(buff);

		
		print_prefix();
	}



	mysys("pwd");
    mysys("echo ,HELLO  WORLD ,  sdfa sdfadf        ss   ");
    mysys("echo /G");
    mysys("echo ,,");
    mysys("echo");



    return 0;
}