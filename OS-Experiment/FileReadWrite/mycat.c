#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd;

    fd = open(argv[1], O_RDONLY);
    
    char buf[2000];
    int count = read(fd, buf, sizeof(buf));
    buf[count] = 0;
    puts(buf);
    
    return 0;
}
