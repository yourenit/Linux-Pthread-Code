#pragma once

#include <cstdio>
#include <list>
#include <pthread.h>
#include <exception>
#include "locker.h"

template< typename T >
class threadpool
{
public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();

    //往请求队列里添加任务
    bool append(T* request);

private:
    //工作线程运行的函数,它不断从工作队列中取出任务病执行
    static void *worker(void * arg);
    void run();

private:
    int m_thread_number;    //线程池中的线程数
    int m_max_requests;     //请求队列中允许的最大请求数目
    pthread_t *m_threads;   //描述线程池的数组，大小为m_thread_number
    std::list< T* > m_workqueue;    //请求队列
    locker m_queuelocker;   //保护请求队列的互斥锁
    sem m_queuestat;        //是否有任务需要处理
    bool m_stop;            //是否结束线程
};

template < typename T >
threadpool<T>::threadpool(int thread_num,int max_request):
    m_thread_num(thread_num),m_max_request(max_request),
    m_stop(false),m_threads(NULL)
{
    if(thread_num <= 0 || max_request <= 0){
        throw std::exception();
    }

    m_threads = new pthread_t[m_thread_num];
    if(!m_threads){
        throw std::exception();
    }

    //创建thread_num个线程，并将它们设置为脱离线程
    for(int i=0;i<thread_num;i++){
        printf("create the %dth thread\n",i);
        if(pthread_create(m_threads+i,NULL,worker,this) != 0){
            delete [] m_threads;
            throw std::exception();
        }

        if(pthread_detach(m_threads[i])){
            delete [] m_threads;
            throw std::exception();
        }
    }
}

template <typename T>
threadpool<T>::~threadpool(){
    delete [] m_pthreads;
    s_stop = true;
}

template <typename T>
bool threadpool<T>::append(T* request){
    //操作工作队列必须加锁，因为它有线程共享
    m_queuelocker.lock();
    //如果工作队列的大小大于允许最大请求数，解锁，并返回错误
    if(m_workqueue.size() > m_max_requests){
        m_queuelocker.unlock();
        return false;
    }

    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    //唤醒阻塞的线程
    m_queuestat.post();
    return true;
}

template <typename T>
void threadpool<T>::worker(void *arg){
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template <typename T>
void threadpool<T>::run(){
    while(!m_stop){
        m_queuestat.wait();
        mqueuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();

        if(!request){
            continue;
        }
        request->process();
    }
}
