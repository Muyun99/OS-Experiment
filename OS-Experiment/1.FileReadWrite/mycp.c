#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
	{
		printf("please check your format, the right format:cp file_src file_target\n");
		exit(0);
	}

    char *sourcePath = argv[1];
    char *targetPath = argv[2];

    int fd1 = open(sourcePath, O_RDONLY);
    int fd2 = open(targetPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
   
	if(fd1 == -1)
	{
		printf("open %s error!",sourcePath);
		exit(0);
	}
	if(fd2 == -1)
	{
		printf("open %s error!",targetPath);
		exit(0);
	}
    char buf[1];
    int count;

    while (read(fd1, buf,1))
    {
        write(fd2, buf, 1);
    }
    close(fd1);
    close(fd2);
    return 0;
}
