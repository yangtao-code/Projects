#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "ftp_server.h"
#include "common.h"
#include "handler.h"
#include "threadpool.h"

//进程退出的标志位
// 1: 进程要退出
// 0:  进程不退出
int terminate = 0;

/***
 * @brief 信号处理函数
 * 
 * @param sig 信号
 */
void sig_handler(int sig)
{
    switch (sig)
    {
    case SIGINT:
        terminate = 1;
        break;
        
    default:
        break;
    }
}


void ftp_server_main(const char *ip, short port)
{
    //注册信号处理程序
    signal(SIGINT, sig_handler);

    //创建套接字并绑定
    int sock = create_listen_sockct(ip, port);
    printf("服务器已开启!\n");

    //创建线程池
    ThreadPool *pool = threadPoolCreate(3, 100, 1024);

    //创建epoll红黑树
    int epfd = epoll_create(1);
    if (epfd == -1)
    {
        perror("epoll_create error");
        close(sock);
        exit(-1);
    }

    //将sock上树
    struct epoll_event ev, evs[1024];
    ev.data.fd = sock;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
    if (ret == -1)
    {
        perror("epoll_ctl error");
    }

    while (!terminate)
    {
        //epoll监听文件描述符的事件
        int n = epoll_wait(epfd, evs, 1024 + 1, 2000);
        //printf("n = %d\n", n);
        //出错退出
        if (n < 0)
        {
            perror("epoll_wait error");
            break;
        }
        //没有变化就继续等待
        else if (n == 0)
        {
            continue;
        }

        //有变化
        for (int i = 0; i < n; i++)
        {
            //如果是sock变化并且是读事件
            if (evs[i].data.fd == sock && evs->events & EPOLLIN)
            {
                struct sockaddr_in client;
                socklen_t addrlen = sizeof(client);
                //处理客户端的连接
                int cfd = Accept(sock, (struct sockaddr *)&client, &addrlen);
                if (cfd > 0)
                {
                    printf("客户端ip: %s  客户端端口号: %d\n",
                           inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                }

                ev.data.fd = cfd;
                ev.events = EPOLLIN; //监听读事件
                //将cfd上树
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                if (ret == -1)
                {
                    perror("epoll_ctl error");
                }
            }
            else if (evs[i].events & EPOLLIN) //普通的读事件
            {
                client_info_t *c = (client_info_t *)malloc(sizeof(client_info_t));
                c->cfd = evs[i].data.fd;
                c->epfd = epfd;
                c->ev = evs[i];
                //让当前客户端的文件描述符下数
                epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &evs[i]);
                //给线程池添加任务
                threadPoolAdd(pool, handle_connection, (void *)c);
            }
        }
    }

    threadPoolDestroy(pool); //销毁线程池
    close(epfd);             //关闭epoll
    close(sock);             //关闭套接字
    printf("服务器已关闭!\n");
}
