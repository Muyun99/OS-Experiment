#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    pid_t pid;
    int fd[2];
    char buf[32];
    pipe(fd);
    pid = fork();
    if(pid == 0){
        dup2(fd[1],1);
        close(fd[0]);
        close(fd[1]);

        execlp("cat","cat","/etc/passwd",NULL);
        exit(0);
    }
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);

    read(0,buf,sizeof(buf));
    execlp("wc","wc","-l",NULL);
    printf("Receive:%s\n",buf);
    return 0;
}
