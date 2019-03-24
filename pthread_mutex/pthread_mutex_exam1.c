#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXNUM 10000

int counter =0;

//创建一把锁
pthread_mutex_t mutex;

void *funcA_num(void *args){
    for(int i=0;i<MAXNUM;i++){
        //加锁
        pthread_mutex_lock(&mutex);
        //int cur=counter;
        //cur++;
        //counter=cur;
        counter++;
        printf("Thread A: id=%lu,counter=%d\n",pthread_self(),counter);
        //解锁
        pthread_mutex_unlock(&mutex);
        usleep(10);
    }
}

void *funcB_num(void *args){
    for(int i=0;i<MAXNUM;i++){
        //加锁
        pthread_mutex_lock(&mutex);
        //int cur=counter;
        //cur++;
        //counter=cur;
        counter++;
        printf("Thread B: id=%lu,counter=%d\n",pthread_self(),counter);
        //解锁
        pthread_mutex_unlock(&mutex);
        usleep(10);
    }
}
int main()
{
    pthread_t tida,tidb;

    pthread_mutex_init(&mutex,NULL);
    pthread_create(&tida,NULL,funcA_num,NULL);
    pthread_create(&tidb,NULL,funcB_num,NULL);

    pthread_join(tida,NULL);
    pthread_join(tidb,NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}

