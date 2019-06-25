#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MAX_ARRAY 100
#define MAX_NUM 10000

void *selectSort(void *argc){
    int *argv = (int *)argc;
    int i,j,min,record = -1,temp;
    for(i = 0;i < MAX_ARRAY/2; ++i){
        min = argv[i];
        for(j = i;j < MAX_ARRAY / 2;++j){
            if(argv[j] < min){
                record = j;
                min = argv[j];
            }
        }
        temp = argv[i];
        argv[i] = argv[record];
        argv[record] = temp;
    }
    return NULL;
}

void Merge(int *arg1, int*arg2){
    int i = 0;
    int j = 0;
    int k = 0;
    for(j = MAX_ARRAY/2;i < MAX_ARRAY/2 && j < MAX_ARRAY;++k){
        if(arg1[i] < arg1[j])
            arg2[k] = arg1[i++];
        else
            arg2[k] = arg1[j++];
    }
    while(i < MAX_ARRAY/2)
        arg2[k++] = arg1[i++];
    while(j < MAX_ARRAY)
        arg2[k++] = arg1[j++];
    
}
void printArray(int *array){
    int i = 0;
    for(;i<MAX_ARRAY;++i)
        printf("%d\n",array[i]);
}
int main(){
    int array[MAX_ARRAY],result[MAX_ARRAY];
    int i;
    for(i = 0;i < MAX_ARRAY; ++i)
        array[i] = (rand() % MAX_NUM);

    printf("INIT ARRAY!\n");
    printArray(array);
    pthread_t tid;
    int *arg = &array[MAX_ARRAY/2];
    selectSort(array);
    pthread_create(&tid, NULL, selectSort, (void *)arg);
    pthread_join(tid,NULL);

    printf("SELECTSORT!\n");
    printArray(array);
    
    Merge(array,result);
    printf("FINAL RESULT!\n");
    printArray(result);
    return 0;
}
