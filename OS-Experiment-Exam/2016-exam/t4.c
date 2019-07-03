#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
int t1_gone = 0,t2_gone = 0,t3_gone = 0,t4_gone = 0;
pthread_mutex_t mutex1,mutex2;
pthread_cond_t signal1,signal2;
void *T1_entry(void *arg)
{
    pthread_mutex_lock(&mutex1);

    sleep(2);
    puts("T1");

    t1_gone = 1;
    pthread_cond_broadcast(&signal1);
    pthread_mutex_unlock(&mutex1);

}
void *T2_entry(void *arg)
{
    pthread_mutex_lock(&mutex1);
    while(!t1_gone)
        pthread_cond_wait(&signal1,&mutex1);
    pthread_mutex_unlock(&mutex1);

    sleep(1);
    puts("T2");

    pthread_mutex_lock(&mutex2);
    t2_gone = 1;
    pthread_cond_signal(&signal2);
    pthread_mutex_unlock(&mutex2);

}
void *T3_entry(void *arg)
{
    pthread_mutex_lock(&mutex1);
    while(!t1_gone)
        pthread_cond_wait(&signal1,&mutex1);
    pthread_mutex_unlock(&mutex1);

    sleep(1);
    puts("T3");

    pthread_mutex_lock(&mutex2);
    t3_gone = 1;
    pthread_cond_signal(&signal2);
    pthread_mutex_unlock(&mutex2);
}
void *T4_entry(void *arg)
{
    pthread_mutex_lock(&mutex2);
    while(!t2_gone || !t3_gone)
        pthread_cond_wait(&signal2,&mutex2);

    puts("T4");

    pthread_mutex_unlock(&mutex2);
}
int main()
{
    pthread_t tids[4];
    pthread_create(&tids[0],0,T1_entry,NULL);
    pthread_create(&tids[1],0,T2_entry,NULL);
    pthread_create(&tids[2],0,T3_entry,NULL);
    pthread_create(&tids[3],0,T4_entry,NULL);

    pthread_join(tids[3],NULL);
}
