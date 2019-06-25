#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUMBER 100000
#define N 100

double PI;
struct param{
    int start;
    int end;
};

struct result{
    double worker_output;
};

void *worker(void *arg){
    int i;
    struct param *param;
    struct result *result;
    double worker_output = 0;
    param = (struct param *) arg;

    for(i = param->start; i <= param->end;i++){
        if(i % 2 == 0)
            worker_output -= 1/(2*(double)i - 1);
        else
            worker_output += 1/(2*(double)i - 1);
    }
    result = malloc(sizeof(struct result));
    result->worker_output = worker_output;
    return result;
}


int main()
{
    int i;
    pthread_t worker_tids[N];
    struct param params[N];
    PI = 0.0;

    for(i = 0; i < N;i++){
        struct param *param;
        param = &params[i];
        param->start =i * NUMBER + 1;
        param->end = (i+1) * NUMBER;
        pthread_create(&worker_tids[i], NULL, worker, param);
    }
    
    for(i = 0;i < N;i++){
        struct result *result;
        pthread_join(worker_tids[i],(void **)&result);
        PI += result->worker_output;
        free(result);
    }
    PI = PI * 4;
    printf("PI:%lf\n",PI);
    return 0;


}
