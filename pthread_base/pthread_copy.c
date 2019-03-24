#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/stat.h>
#include <pthread.h>

int pthread_num=5;
static void * ptr;
static void * ptr2;
static size_t file_size;

void * p_copy(void *index){

    int i=*(int *)index;
printf("线程%d---文件大小：%ld\n",i,file_size);
    int srcpoint =(file_size/pthread_num)*i;
printf("已复制字节数：%d",srcpoint);
    int destpoint = srcpoint;

    printf("%d\n",srcpoint);

    memcpy(ptr2+destpoint,ptr+srcpoint,file_size/pthread_num);

    pthread_exit(NULL);
}

int main(int argc,char *argv[]){
    int pthread_num;
    if(argc < 3 || argc >4){
        printf("Usage:./a.out SRCFILE DESTFILE [THREAD_NUM]\n");
        exit(1);
    }else if(argc == 3){
        pthread_num=5;
    }else{
        pthread_num=atoi(argv[3]);
    }

    int fd,fd2;

    //打开源文件
    fd=open(argv[1],O_RDWR);
    if(fd < 0){
        perror("open");
        exit(1);
    }

    //获取文件大小
    file_size=lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);

    //源文件映射
    ptr = mmap(0,file_size,PROT_READ,MAP_PRIVATE,fd,0);
    //打开目的文件
    fd2 =open(argv[2],O_CREAT|O_RDWR);
    if(fd < 0){
        perror("open dest");
        exit(1);
    }

    //文件拓展
    write(fd2,"1",file_size);
    //目标文件映射
    ptr2 =mmap(0,file_size,PROT_WRITE,MAP_SHARED,fd2,0);

    pthread_t pth[pthread_num];
    for(int i=0;i<pthread_num;i++){
        pthread_create(&pth[i],NULL,p_copy,&i);
        usleep(1000);
    }
    printf("start copying\n");

    //线程属性为可回收PTHREAD_CREATE_JOINABLE
    for(int i=0;i<pthread_num;i++){
        pthread_join(pth[i],NULL);
    }

    printf("Done!\n");
    return 0;
}
