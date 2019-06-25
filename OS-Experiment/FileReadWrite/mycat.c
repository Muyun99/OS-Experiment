#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
	int fd;
	char c[1];
    if(argc == 1)
	{
		while(read(0,c,1))
			write(1,c,1);
	}
	else
	{
		int i = 0;
		for(i = 1;i < argc;i++)
		{
		    fd = open(argv[i], O_RDONLY);
			if(fd == -1) //file open error
			{
				printf("mycat: %s:No such file or directory\n", argv[i]);
				continue;
			}
			while(read(fd,c,1))
				write(1,c,1);
			close(fd);
		}
    }
    return 0;
}
