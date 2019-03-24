#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//全局变量counter
int counter = 0;

//定义一个读写锁
pthread_rwlock_t mutex;

void *write_func(void *arg){
    while(1){
        pthread_rwlock_wrlock(&mutex);
        counter++;
        printf("==write: %lu,counter=%d\n",pthread_self(),counter);
        pthread_rwlock_unlock(&mutex);
        usleep(500);
    }
}

void *read_func(void *arg){
    while(1){
        pthread_rwlock_rdlock(&mutex);
        printf("==read: %lu,counter=%d\n",pthread_self(),counter);
        pthread_rwlock_unlock(&mutex);
        usleep(500);
    }
}

int main()
{
    pthread_t pth[8];

    pthread_rwlock_init(&mutex,NULL);

    //创建三个写线程
    for(int i=0; i<3; i++)
        pthread_create(&pth[i],NULL,write_func,NULL);
    //创建5个读线程
    for(int i=3; i<8;i++)
        pthread_create(&pth[i],NULL,read_func,NULL);

    //阻塞回收子线程
    for(int i=0;i<8;i++)
        pthread_join(pth[i],NULL);

    pthread_rwlock_destroy(&mutex);
    return 0;
}

