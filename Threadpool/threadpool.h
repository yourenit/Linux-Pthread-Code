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

    bool append(T* request);

private:
    int m_thread_number;    //线程池中的线程数
    int m_max_requests;     //请求队列中允许的最大请求数目
    pthread_t *m_threads;   //描述线程池的数组，大小为m_thread_number
    std::list< T* > m_workqueue;    //请求队列
    locker m_queuelocker;   //保护请求队列的互斥锁
    sem m_queuestat;        //是否有任务需要处理
    bool m_stop;            //是否结束线程
};

