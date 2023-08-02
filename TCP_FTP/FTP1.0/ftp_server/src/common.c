#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int create_listen_sockct(const char *ip, short port)
{
    int sock;
    int ret;
    //1.创建一个套接字
    sock = Socket(AF_INET, SOCK_STREAM, 0);

    //设置选项,允许重用IP地址
    int on = 1;
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&on, sizeof(on));
    if (ret == -1)
    {
        perror("failed to setsockopt");
        return -1;
    }

    //设置选项，允许重用端口
    on = 1;
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (const void *)&on, sizeof(on));
    if (ret == -1)
    {
        perror("failed to setsockopt");
        return -1;
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr(ip);

    Bind(sock, (struct sockaddr *)&sa, sizeof(sa));

    Listen(sock, 10);

    return sock;
}

int connect_tcp_server(char *serv_ip, short port)
{
    int sock;
    //1.创建一个套接字
    sock = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr(serv_ip);

    Connect(sock, (struct sockaddr *)&sa, sizeof(sa));

    return sock;
}

int Packet_reception(int sock_fd,unsigned char* buf, int len)
{
    //1.接收数据
    unsigned char recv_data[8192] = "";

    unsigned char ch;
    int ret;
    do
    {
        ret = read(sock_fd, &ch, 1);

    } while (ch != 0xc0);

    while (ch == 0xc0)
    {
        read(sock_fd, &ch, 1);
    }

    int recv_data_len = 0;
    while (ch != 0xc0)
    {
        recv_data[recv_data_len++] = ch;
        read(sock_fd, &ch, 1);
    }
    

    //2. 转义:
    //   0xdb 0xdc ---> 0xc0
    //   0xdb 0xdd ---> 0xdb
    int k = 0;
    for (int i = 0; i < recv_data_len; i++)
    {
        if (recv_data[i] == 0xdb && i != recv_data_len - 1)
        {
            if (recv_data[i + 1] == 0xdc)
            {
                buf[k++] = 0xc0;
                i++;
            }
            else if (recv_data[i + 1] == 0xdd)
            {
                buf[k++] = 0xdb;
                i++;
            }
            else
            {
                buf[k++] = recv_data[i];
            }
        }
        else
        {
            buf[k++] = recv_data[i];
        }
    }
    
    buf[k] = '\0';
    //printf("recv_raw_len = %d\n",k);
    return k;
}

int Packet_transmission(int sock_fd, const unsigned char *buf, int len)
{
    unsigned char *send_data = malloc(2 + 2 * len);
    memset(send_data,0,2 + 2 * len);
    int n = 0;
    send_data[n++] = 0xc0;
    for (int i = 0; i < len; i++)
    {
        if (buf[i] == 0xc0)
        {
            send_data[n++] = 0xdb;
            send_data[n++] = 0xdc;
        }
        else if (buf[i] == 0xdb)
        {
            send_data[n++] = 0xdb;
            send_data[n++] = 0xdd;
        }
        else
        {
            send_data[n++] = buf[i];
        }
    }
    send_data[n++] = 0xc0;
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%02x ", send_data[i]);
    // }
    // printf("\n");

    int ret = send(sock_fd, send_data, n, 0);
    if (ret != n)
    {
        perror("send error");
        return ret;
    }
    free(send_data);
    return len;
}

void perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int n;
    while ((n = accept(fd, sa, salenptr)) < 0)
    {
        if ((errno == ECONNABORTED) || (errno == EINTR)) // 如果是被信号中断和软件层次中断,不能退出
        {
            continue;
        }
        else
        {
            perror("accept error");
            return -1;
        }
    }
    return n;
}

int Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    int n;

    if ((n = bind(fd, sa, salen)) < 0)
        perr_exit("bind error");

    return n;
}

int Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
    int n;

    if ((n = connect(fd, sa, salen)) < 0)
        perr_exit("connect error");

    return n;
}

int Listen(int fd, int backlog)
{
    int n;

    if ((n = listen(fd, backlog)) < 0)
        perr_exit("listen error");

    return n;
}

int Socket(int family, int type, int protocol)
{
    int n;

    if ((n = socket(family, type, protocol)) < 0)
        perr_exit("socket error");

    return n;
}