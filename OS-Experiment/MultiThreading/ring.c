#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define N 100
int buffer = 0;

void *add(void *arg){
    int *num = (int *)arg;
    num[0]++;
    int *result = num;
    return (void *)result;
}

void init(int a[N][2]){
    int i;
    for(i = 0;i < N;i++){
        a[i][0] = 0;
        a[i][1] = i;
    }
}


int main(){
    int i = 0;
    int array[N][2];
    init(array);
    int *result;

    pthread_t tids[N];
    pthread_create(&tids[0],NULL,add,(void *)array[0]);
    pthread_join(tids[0], (void *)&result);
    
    while(i < N){
        printf("from T[%d]", i+1);
        i = (i+1) % N;
        printf("to T[%d] send %d\n",i+1,result[0]);
        pthread_create(&tids[i],NULL,add,result);
        pthread_join(tids[i], (void *)&result);
		if(i == 0)
			break;
       // sleep(1);
    } 

    return 0;
}
