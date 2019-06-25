#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    
    char *sourcePath = argv[1];
    char *targetPath = argv[2];

    int fd1 = open(sourcePath, O_RDONLY);
    int fd2 = open(targetPath, O_WRONLY | O_CREAT | O_TRUNC);
   
    char buf[2000];
    int count;

    while((count = read(fd1, buf, sizeof(buf))) > 0)
    {
        write(fd2, buf, count);
    }
    close(fd1);
    close(fd2);
    return 0;
}
