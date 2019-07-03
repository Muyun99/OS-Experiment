#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int computeResult(int start,int end)
{
    int result = 0;
    for(int i = start;i <= end;i++)
        result += i;
    return result;
}

int main()
{
    pid_t pid1,pid2;
    pid1 = -1;
    pid2 = -1;
    int fd1[2],fd2[2];
    pipe(fd1);
    pipe(fd2);


    pid1 = fork();
    if(pid1 > 0)
    {
        pid2 = fork();
    }
    if(pid1 == 0) //child1
    {
        int result1 =  computeResult(1,50);
        printf("result1:%d\n",result1);
        write(fd1[1],&result1,sizeof(result1));
        exit(1);
    }
    if(pid2 == 0)//child2
    {
        int result2 = computeResult(51,100);
        printf("result23:%d\n",result2);
        write(fd2[1],&result2,sizeof(result2));
        exit(1);
    }
    if(pid1 > 0)
    {
        int result1,result2;
        read(fd1[0],&result1,sizeof(result1));
        read(fd2[0],&result2,sizeof(result2));

        int result = result1 + result2;
        printf("result:%d\n",result);
    }
    return 0;
}
