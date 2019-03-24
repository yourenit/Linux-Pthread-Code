#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//定义信号量
sem_t producer;
sem_t customer;
//定义节点结构体
typedef struct node{
    int data;
    struct node* next;
}Node;

Node* head=NULL;

void *producer_func(void* arg){
    while(1){
        sem_wait(&producer);
        Node* p=(Node*)malloc(sizeof(Node));
        p->data = rand() % 1000;
        //对共享数据进行加锁
        p->next = head;
        head = p;
        printf("=====producer:%lu,%d\n",pthread_self(),p->data);

        sem_post(&customer);
        sleep(rand()%3);
    }
}

void *customer_func(void* arg){
    while(1){
        if(head == NULL){
            sem_wait(&customer);
        }
        Node* p=head;
        head=head->next;
        printf("=====customer:%lu,%d\n",pthread_self(),p->data);
        free(p);
        sem_post(&producer);
    }
}
int main()
{
    pthread_t p1,p2;

    //初始化信号量
    sem_init(&producer,0,2);
    sem_init(&customer,0,0);
    //创建生产者和消费者线程
    pthread_create(&p1,NULL,producer_func,NULL);
    
    pthread_create(&p2,NULL,customer_func,NULL);

    //阻塞回收线程
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    //销毁信号量
    pthread_mutex_destroy(&mutex);
    sem_destroy(&producer);
    sem_destroy(&customer);
    return 0;
}

