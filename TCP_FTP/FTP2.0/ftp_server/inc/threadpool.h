#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 任务结构体
typedef struct Task
{
    void (*function)(void *); // 回调函数
    void *arg;                   // 参数
} Task;

// 线程池结构体
typedef struct ThreadPool
{
    // 任务队列
    Task *taskQ;
    int queueCapacity; // 容量
    int queueSize;     // 当前任务个数
    int queueFront;    // 对头  ->  取数据
    int queueRear;     // 队尾  ->  存数据

    pthread_t managerID;  // 管理者线程id
    pthread_t *threadIDs; // 工作的线程id
    int minNum;           // 最小线程数量
    int maxNum;           // 最大线程数量
    int busyNum;          // 忙的线程数量
    int liveNum;          // 存活的线程数量
    int exitNum;          // 要销毁的线程数量

    pthread_mutex_t mutexPool; // 锁整个的线程池
    pthread_mutex_t mutexBusy; // 锁busyNum变量
    pthread_cond_t notFull;    // 任务变量是否满了
    pthread_cond_t notEmpty;   // 任务变量是否空了

    int shutdown; // 线程池是否关闭
} ThreadPool;

// 工作者线程函数
void *worker(void *arg);
// 管理者线程函数
void *manager(void *arg);

/***
 * @brief 线程销毁
 * 
 * @param pool 线程池
 */
void threadExit(ThreadPool* pool);

/***
 * @brief 创建线程池并初始化
 * 
 * @param min 线程池中最小线程数
 * @param max 线程池中最大线程数
 * @param queueSize 任务队列的最大任务数
 * @return ThreadPool* 成功返回一个线程池地址,失败返回NULL
 */
ThreadPool* threadPoolCreate(int min, int max, int queueSize);

/***
 * @brief 销毁线程池,如果线程池内部还有任务没完成,会堵塞到任务完成
 * 
 * @param pool 线程池
 * @return int 成功返回0,失败返回-1
 */
int threadPoolDestroy(ThreadPool* pool);

/***
 * @brief 给线程池添加任务,任务参数需要从堆区开辟,线程池会自动释放参数
 * 
 * @param pool 线程池的权柄
 * @param func 回调函数
 * @param arg  回调函数参数
 */
void threadPoolAdd(ThreadPool* pool,void(*func)(void*),void* arg);

/***
 * @brief 获取线程池中工作的线程个数
 * 
 * @param pool 线程池
 * @return int 返回线程池中工作的线程个数
 */
int threadPoolBusyNum(ThreadPool* pool);

/***
 * @brief 获取线程池中存活的线程个数
 * 
 * @param pool 线程池
 * @return int 返回线程池中存活的线程个数
 */
int threadPoolliveNum(ThreadPool* pool);

#endif