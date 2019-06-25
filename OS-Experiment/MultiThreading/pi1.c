#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define NUMBER 100000

double PI;
double worker_output;
double master_output;

void *worker(void *arg){
    int i;
    worker_output = 0;
    for(i = 1; i <= NUMBER;i++){
        if(i % 2 == 0)
            worker_output -= 1/(2*(double)i - 1);
        else
            worker_output += 1/(2*(double)i - 1);
    }
}

void master(){
    int i;
    master_output = 0;
    for(i = NUMBER + 1;i <= NUMBER*2;i++){
        if(i % 2 == 0)
            master_output -= 1 / (2 * (double)i - 1);
        else
            master_output += 1 / (2 * (double)i - 1);
    }
}

int main()
{
    pthread_t worker_tid;
    pthread_create(&worker_tid, NULL, &worker, NULL);
    master();
    pthread_join(worker_tid,NULL);
    PI = (worker_output + master_output) * 4;
    printf("PI:%lf\n",PI);
    return 0;


}
