#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define N 10
struct param{
    int start;
    int end;
};
struct result{
    int sum;
};
void *add(void *arg)
{
    struct param *param = (struct param*) arg;
    struct result *result = malloc(sizeof(struct result));
    result->sum = 0;

    for(int i = param->start;i <= param->end;i++)
        result->sum += i;
    return result;
}
int main()
{
    int i = 0;
    struct param params[N];
    struct result result[N];
    pthread_t tids[N];
    int finalresult = 0;
    for(i = 0;i < N;i++)
    {
        struct param *param = &params[i];
        struct result *result = malloc(sizeof(struct result));
        param->start = i * 10 + 1;
        param->end = (i + 1) * 10;
        pthread_create(&tids[i],NULL,add,(void *)param);
        pthread_join(tids[i],(void **)&result);
        finalresult += result->sum;
        free(result);
    }
    printf("%d",finalresult);
    //printf("result: %d",result);
    return 0;
}
