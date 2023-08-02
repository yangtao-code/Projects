#include "threadpool.h"

//一次要关闭的线程数
const int NUMBER = 2;

void *worker(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;

    while (1)
    {
        int ret = pthread_mutex_lock(&pool->mutexPool);
        // 当前任务队列为空
        while (pool->queueSize == 0 && !pool->shutdown)
        {
            // 堵塞工作线程
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                // 判断线程池中的线程数是否大于最小线程数

                pool->liveNum--;
                pthread_mutex_unlock(&pool->mutexPool);
                threadExit(pool);
            }
        }
        // 判断线程池是否已经关闭了
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        // 从任务队列中取出任务
        Task task;
        task.function = pool->taskQ[pool->queueFront].function;
        task.arg = pool->taskQ[pool->queueFront].arg;
        // 移动头结点
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize--;
        // 解锁
        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);

        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        // 执行任务
        printf("thread %ld working...\n",pthread_self());
        task.function(task.arg);
        free(task.arg);
        task.arg = NULL;
        printf("thread %ld end work...\n",pthread_self());
        
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

void *manager(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;
    while (!pool->shutdown)
    {
        // 每隔1s检测一次
        sleep(1);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        // 取出忙的线程的数量
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        //printf("任务个数%d,存活线程个数%d,工作线程个数%d\n",queueSize,liveNum,busyNum);

        // 增加线程
        // 任务的个数 > 存活的线程个数 && 存活的线程数 < 最大的线程数
        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER && pool->liveNum < pool->maxNum; ++i)
            {
                if (pool->threadIDs[i] == 0)
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, (void *)pool);
                    pthread_detach(pool->threadIDs[i]);
                    printf("thread %ld create...\n", pool->threadIDs[i]);
                    counter++;
                    pool->liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // 销毁线程
        // 忙的线程*2 < 存活的线程 && 存活的线程 > 最小的线程数
        if (busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);

            // 让工作的线程自杀
            for (int i = 0; i < NUMBER; i++)
            {
                if (pool->liveNum > pool->minNum)
                {
                    pthread_cond_signal(&pool->notEmpty);
                }
            }
            pool->exitNum = 0;
        }
    }
    return NULL;
}

void threadExit(ThreadPool *pool)
{
    //获取当前线程的线程号
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; i++)
    {
        //找到当前线程号并置为0
        if (tid == pool->threadIDs[i])
        {
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting...\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}

ThreadPool *threadPoolCreate(int min, int max, int queueSize)
{
    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    do
    {
        if (NULL == pool)
        {
            perror("malloc threadpool failed");
            break;
        }
        pool->threadIDs = (pthread_t *)malloc(sizeof(pthread_t) * max);
        if (NULL == pool->threadIDs)
        {
            perror("malloc threadIDs failed");
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * max); // 将工作线程的id初始化为0
        pool->minNum = min;
        pool->maxNum = max;
        pool->busyNum = 0;
        pool->liveNum = min; // 和最小数相等
        pool->exitNum = 0;

        if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
            pthread_cond_init(&pool->notFull, NULL) != 0)
        {
            printf("mutex or condition init fail...\n");
            break;
        }

        // 任务队列
        pool->taskQ = (Task *)malloc(sizeof(Task) * queueSize);
        pool->queueCapacity = queueSize;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;

        pool->shutdown = 0;

        // 创建线程
        pthread_create(&pool->managerID, NULL, manager, (void *)pool);
        for (int i = 0; i < min; i++)
        {
            pthread_create(&pool->threadIDs[i], NULL, worker, (void *)pool);
            //设置线程分离属性
            pthread_detach(pool->threadIDs[i]);
            printf("thread %ld create...\n", pool->threadIDs[i]);
        }
        printf("线程池创建成功\n");
        return pool;
    } while (0);
    // 释放资源
    if (pool != NULL && pool->taskQ != NULL)
        free(pool->taskQ);
    if (pool != NULL && pool->threadIDs != NULL)
        free(pool->threadIDs);
    if (pool != NULL)
        free(pool);
    return NULL;
}

void threadPoolAdd(ThreadPool *pool, void (*func)(void *), void *arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while (pool->queueCapacity == pool->queueSize && !pool->shutdown)
    {
        // 阻塞生产者线程
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }

    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    // 添加任务
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;
    pthread_cond_signal(&pool->notEmpty);
    
    pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolliveNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int liveNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return liveNum;
}

int threadPoolDestroy(ThreadPool *pool)
{
    if (pool == NULL)
    {
        return -1;
    }
    // 关闭线程池
    pool->shutdown = 1;
    // 阻塞回收管理者线程
    pthread_join(pool->managerID, NULL);

    // 唤醒阻塞的消费者线程
    printf("存活的线程数为%d,正在让线程自杀\n", pool->liveNum);
    for (int i = 0; i < pool->liveNum; i++)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    sleep(2);
    // 释放堆内存
    if (pool->taskQ)
    {
        free(pool->taskQ);
    }
    if (pool->threadIDs)
    {
        free(pool->threadIDs);
    }
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    free(pool);
    pool = NULL;
    return 0;
}