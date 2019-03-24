#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//定义互斥锁和条件变量
pthread_mutex_t mutex;
pthread_cond_t cond;
//定义节点结构体
typedef struct node{
    int data;
    struct node* next;
}Node;

Node* head=NULL;

void *producer(void* arg){
    while(1){
        Node* p=(Node*)malloc(sizeof(Node));
        p->data = rand() % 1000;
        //对共享数据进行加锁
        pthread_mutex_lock(&mutex);
        p->next = head;
        head = p;
        printf("=====producer:%lu,%d\n",pthread_self(),p->data);
        pthread_mutex_unlock(&mutex);

        //对阻塞的线程进行唤醒
        pthread_cond_signal(&cond);
        sleep(rand()%3);
    }
}

void *customer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(head == NULL){
            //对线程进行阻塞，并且对锁进行解锁
            pthread_cond_wait(&cond,&mutex);
            //解除阻塞之后，对锁进行加锁
        }
        Node* p=head;
        head=head->next;
        printf("=====customer:%lu,%d\n",pthread_self(),p->data);
        free(p);
        pthread_mutex_unlock(&mutex);
    }
}
int main()
{
    pthread_t p1,p2;

    //初始化互斥锁和条件变量
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    //创建生产者和消费者线程
    pthread_create(&p1,NULL,producer,NULL);
    
    pthread_create(&p2,NULL,customer,NULL);

    //阻塞回收线程
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    //销毁互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

