#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include "common.h"
#include "handler.h"

#define FTP_PATH "/home/china/code/0-tmp/"

void handle_connection(void *arg)
{
    client_info_t *c = (client_info_t *)arg;
    //1. 接受一个完整的 ftp 命令
    unsigned char raw_cmd[100] = "";

    //接收客户端发过来的命令
    Packet_reception(c->cfd, raw_cmd, 100);
    printf("收到命令: ");

    //3. 处理命令
    //命令参数长度
    int arg_len = 0;
    for (int i = 1; i <= 4; i++)
    {
        arg_len += raw_cmd[i] << ((i - 1) * 8);
    }

    if (arg_len == 0)
    {
        handle_cmd(c, raw_cmd[0], NULL, 0);
    }
    else
    {
        unsigned char *arg = raw_cmd + 5;
        handle_cmd(c, raw_cmd[0], arg, arg_len);
    }
}

void handle_cmd(client_info_t *c, cmd_no cmd, const unsigned char *arg, int arg_len)
{
    switch (cmd)
    {
    case CMD_LS: //ls命令
        printf("ls\n");
        handle_cmd_ls(c->cfd, arg, arg_len);
        break;
    case CMD_GET: //get命令
        printf("get\n");
        handle_cmd_get(c->cfd, arg, arg_len);
        break;
    case CMD_PUT: //put命令
        printf("put\n");
        handle_cmd_put(c->cfd, arg, arg_len);
        break;
    case CMD_BYE: //bye命令
        printf("bye\n");
        handle_cmd_bye(c->cfd, arg, arg_len);
        //bye命令直接返回,不用再将客户端的文件描述符上树了
        return;
    default:
        break;
    }
    //将客户端的文件描述符重新上树
    epoll_ctl(c->epfd, EPOLL_CTL_ADD, c->cfd, &c->ev);
}

void handle_cmd_ls(int cfd, const unsigned char *arg, int arg_len)
{
    //打开目录
    DIR *dir = opendir(FTP_PATH);
    //回复客户端执行结果
    if (dir == NULL)
    {
        perror("opendir error");
        send_execution_result(cfd, CMD_LS, 0x11);
        return;
    }
    send_execution_result(cfd, CMD_LS, 0x00);

    struct dirent *d;
    unsigned char data[4096] = "";
    while (d = readdir(dir))
    {
        //如果是.或者..或者目录就不用添加到name里面
        if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..") || d->d_type == DT_DIR)
        {
            continue;
        }
        struct stat s;
        char name[300] = "";
        sprintf(name, "%s ", d->d_name);
        strcat(data, name);
    }
    //发生文件名给客户端
    send_cmd_data(cfd, CMD_LS, data, strlen(data));
}

void handle_cmd_get(int cfd, const unsigned char *arg, int arg_len)
{
    char filepath[256] = "";
    sprintf(filepath, "%s/%s", FTP_PATH, arg);
    //判断文件的状态
    struct stat s;
    int ret = stat(filepath, &s);
    if (ret == -1)
    {
        perror("stat error");
        send_execution_result(cfd, CMD_GET, 0xc0);
        return;
    }
    //文件长度
    int file_len = s.st_size;
    int send_fd = open(filepath, O_RDONLY);
    if (send_fd == -1)
    {
        perror("open file error");
        send_execution_result(cfd, CMD_GET, 0x11);
    }

    send_execution_result(cfd, CMD_GET, 0x00);
    //发生文件长度给客户端
    send_file_len(cfd, file_len);
    //发生文件
    send_file(cfd, send_fd, file_len);
}

void send_file_len(int cfd, int file_len)
{
    unsigned char raw_data[4] = "";
    for (int i = 0; i < 4; i++)
    {
        raw_data[i] = file_len >> (8 * i);
    }

    int ret = Packet_transmission(cfd, raw_data, 4);
    if (ret == -1)
    {
        printf("send len error \n");
    }
}

void send_file(int cfd, int send_fd, int file_len)
{
    unsigned char *buf = (unsigned char *)malloc(4096);
    if (buf == NULL)
    {
        perror("malloc error");
        return;
    }

    int len = 0;
    int n;
    lseek(send_fd, SEEK_SET, 0);
    while (len < file_len)
    {
        memset(buf, 0, 4096);
        n = read(send_fd, buf, 4096);

        n = Packet_transmission(cfd, buf, n);
        len += n;
        //printf("文件大小%d,已发送 %d\n", file_len, len);
    }
    printf("发送完毕!\n");
    free(buf);
}

void handle_cmd_put(int cfd, const unsigned char *arg, int arg_len)
{
    //发送命令结果给客户端
    send_execution_result(cfd, CMD_PUT, 0x00);

    //接收文件大小数据包
    unsigned char data[10] = "";
    int ret = Packet_reception(cfd, data, 10);

    int file_len = 0;
    for (int i = 0; i < 4; i++)
    {
        file_len += data[i] << (i * 8);
    }

    //接收文件
    recv_ftp_file(cfd, arg, file_len);
}

int recv_ftp_file(int cfd, const char *filename, int file_len)
{
    char filepath[100] = "";

    sprintf(filepath, "%s/%s", FTP_PATH, filename);

    int fd = open(filepath, O_CREAT | O_WRONLY, 0777);
    if (fd == -1)
    {
        perror("open error");
        return -1;
    }
    int n = 0;
    unsigned char *data = (unsigned char *)malloc(4096);
    printf("接收 %s 文件,文件大小为 %d\n", filename, file_len);
    while (n < file_len)
    {
        memset(data, 0, 4096);
        int len = Packet_reception(cfd, data, 4096);

        //printf("len = %d\n", len);
        int ret = write(fd, data, len);
        n += ret;
        //printf("文件大小: %d,已接收: %d\n", file_len, n);
    }
    printf("接收完毕!\n");
    free(data);
    close(fd);
    return 0;
}

void handle_cmd_bye(int cfd, const unsigned char *arg, int arg_len)
{
    send_execution_result(cfd, CMD_BYE, 0x00);
    printf("客户端断开连接!\n");
    close(cfd);
}

void send_cmd_data(int cfd, cmd_no cmd, char *data, int data_len)
{
    //printf("data_len = %d\n",data_len);
    int raw_data_len = 1 + 4 + data_len;
    unsigned char *raw_data = (unsigned char *)malloc(raw_data_len);
    int k = 0;
    //数据包内容 0xc0 命令号 内容长度 内容 0xc0
    raw_data[k++] = cmd;
    for (int i = 0; i < 4; i++)
    {
        raw_data[k++] = (data_len >> (i * 8) & 0xff);
    }

    for (int i = 0; i < data_len; i++)
    {
        raw_data[k++] = data[i];
    }

    int ret = Packet_transmission(cfd, raw_data, k);
    //printf("send %d\n", ret);
    if (ret != k)
    {
        printf("send error\n");
    }
    free(raw_data);
}

void send_execution_result(int cfd, cmd_no cmd, unsigned char result)
{
    //要发送的命令执行结果数据包格式 0xc0 cmd result 0xc0 四个字节
    unsigned char send_result[2] = "";

    send_result[0] = cmd;
    send_result[1] = result;

    Packet_transmission(cfd, send_result, 2);
}