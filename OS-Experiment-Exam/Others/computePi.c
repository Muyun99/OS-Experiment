#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUMBER 1000000
#define WORKERNUM 100

struct param{
    int start;
    int end;
    double result;
};

void *worker(void *arg){
    int i;
    struct param *param =  (struct param *) arg;
    for (i = param->start; i <= param->end;i++){
        if(i%2 == 0)
            param->result -= 1/(2 * (double)i - 1);
        else
            param->result += 1/(2 * (double)i - 1);
    }

}



void master()
{
    int i;
    pthread_t worker_tids[WORKERNUM];
    struct param params[WORKERNUM];
    double  PI = 0.0;

    for(i = 0;i < WORKERNUM;i++)
    {
        struct param *param = &params[i];
        param->start = i * NUMBER + 1;
        param->end = (i+1) * NUMBER;
        param->result = 0;
        pthread_create(&worker_tids[i],NULL,worker,(void *)&params[i]);
        pthread_join(worker_tids[i],NULL);
        PI += param->result;
    }
    PI = PI * 4;
    printf("PI;%lf\n",PI);
}

int main()
{
    master();
    return 0;
}
