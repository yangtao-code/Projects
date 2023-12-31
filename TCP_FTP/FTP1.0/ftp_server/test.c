#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_func(void *arg)
{
    int oldstate;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
    printf("Thread is running...\n");
    sleep(5);
    printf("Thread is done.\n");
    pthread_setcancelstate(oldstate, NULL);
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, thread_func, NULL);
    if (ret != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    printf("Main thread is running...\n");
    sleep(2);
    printf("Main thread is requesting cancel...\n");
    pthread_cancel(thread);
    pthread_join(thread, NULL);
    printf("Main thread is done.\n");
    retu   1.txt