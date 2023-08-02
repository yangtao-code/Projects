#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/poll.h>

#include "ftp_server.h"
#include "common.h"
#include "handler.h"

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
    printf("服务器创建成功!\n");

    while (!terminate)
    {
        struct pollfd fds[1];
        fds[0].fd = sock;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        int ret = poll(fds, 1, 2000);
        if (ret <= 0)
        {
            continue;
        }

        if (fds[0].revents & POLLIN)
        {
            struct sockaddr_in client;
            socklen_t addrlen = sizeof(client);

            int cfd = Accept(sock, (struct sockaddr *)&client, &addrlen);
            if (cfd > 0)
            {
                printf("客户端ip: %s  客户端端口号: %d\n",
                       inet_ntoa(client.sin_addr), ntohs(client.sin_port));

                pid_t pid = fork();
                if (pid == 0)
                {
                    handle_connection(cfd);
                    exit(0);
                }
                else if (pid > 0)
                {
                    //父进程
                    close(cfd);
                }
            }
        }
    }
}
