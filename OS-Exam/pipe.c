#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    int std_in = dup(0);
    int std_out = dup(1);
    pid_t tid1 = -1,tid2 = -1;
    int fd1[2],fd2[2];
    pipe(fd1);
    pipe(fd2);

    tid1 = fork();
    if(tid1 > 0)
    {
        tid2 = fork();
    }
    if(tid1 == 0) //cat
    {
        dup2(fd1[1],1);
        close(fd1[0]);
        close(fd1[1]);
        execlp("cat","cat","/etc/passwd",NULL);
    }
    if(tid2 == 0) //grep
    {
        dup2(fd1[0],0);
        close(fd1[0]);
        close(fd1[1]);
        // char buff[1024];
        // int readsize = read(fd[0],buff,sizeof(buff));
        // write(fd2[1],buff,readsize);

        dup2(fd2[1],1);
        close(fd2[0]);
        close(fd2[1]);

        execlp("grep","grep","root",NULL);
    }
    if(tid1>0) //wc
    {
        dup2(fd2[0],0);
        close(fd2[0]);
        close(fd2[1]);

        dup2(std_out,1);
        execlp("wc","wc","-l",NULL);
    }
    return 0;
}
