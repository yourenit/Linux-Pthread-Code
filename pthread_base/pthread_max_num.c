#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 0x100000

void *fun(){
    int n=3;
    while(n--){
       // printf("hello\n");
       sleep(1);
    }
}

int main(){
    pthread_t tid;
    int err,detastate,i=1;
    size_t stacksize;
    void *stackaddr;

    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_attr_getstack(&attr,&stackaddr,&stacksize);
    printf("stackaddr=%p\n",stackaddr);
    printf("stacksize=%x\n",(int)stacksize);

    pthread_attr_getdetachstate(&attr,&detastate);
    if(detastate == PTHREAD_CREATE_DETACHED)
        printf("thread detached\n");
    else if(detastate == PTHREAD_CREATE_JOINABLE)
        printf("thread join\n");
    else
        printf("thread unknow\n");

    //设置线程分离属性
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    while(1){
        stackaddr = malloc(SIZE);
        if(stackaddr == NULL){
            perror("malloc");
            exit(1);
        }
        stacksize=SIZE;
        pthread_attr_setstack(&attr,stackaddr,stacksize);
        err=pthread_create(&tid,&attr,fun,NULL);
        if(err!=0){
            printf("%s\n",strerror(err));
            exit(1);
        }
        printf("%d\n",i++);
    }
    pthread_attr_destroy(&attr);
    return 0;
}
