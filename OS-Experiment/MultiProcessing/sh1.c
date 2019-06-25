#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

void mysys(char *command)
{
    int pid;
    pid = fork();
    if(pid == 0)
    {
        
        
        char *argv[100];
        char *token;
        char cmd[sizeof(command) + 1];
        strcpy(cmd, command);
    
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
    char ch[100];
    while(1)
    {
        printf("> ");
        gets(ch);
        if(strcmp(ch, "exit") == 0)
        {
            break;
        }
        else
            mysys(ch);
    }
    return 0;
}
