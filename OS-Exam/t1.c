#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    int fd[2];
    char buff[32];
    pipe(fd);


    pid_t tid;
    tid = fork();

    if(tid == 0)//children
    {
        dup2(fd[1],1);
        close(fd[0]);
        close(fd[1]);
        execlp("echo","echo","hello wolrd",NULL);
        exit(0);
    }
    else//parent
    {

        dup2(fd[0],0);
        close(fd[0]);
        close(fd[1]);
        int readsize = read(0,buff,sizeof(buff));
        write(1,buff,readsize);
    }
    return 0;
}
