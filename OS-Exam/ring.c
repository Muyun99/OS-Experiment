#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define N 10
#define LOOPCOUNT 25

void *add(void *arg){
    int *num = (int *)arg;
    num[1] = num[0] + 1;
    int *result = num;
}

int main()
{
    int buff[N][2];
    int i = 0;
    for(i = 0;i < N;i++)
    {
        buff[i][0]=0;
        buff[i][1]=0;
    }
    pthread_t tids[N];

    i = 0;
    int count = 0;
    while(i < N)
    {
        count++;
        if(count == LOOPCOUNT)
            break;

        printf("from T[%d]",i+1);
        pthread_create(&tids[i],NULL,add,(void *)&buff[i]);
        pthread_join(tids[i],NULL);
        int result = buff[i][1];

        i = (i+1) % N;
        buff[i][0] = result;
        printf("to T[%d] send %d\n",i+1,result);
    }
    return 0;
}
