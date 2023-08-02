#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include "ftp_client.h"

#define FTP_PATH "/home/china/ftp"

static int sock;

//进程退出的标志位
// 1： 进程要退出
// 0:  进程不退出
int terminate = 0;

void sig_handler(int sig)
{
    switch (sig)
    {
    case SIGINT:
        handle_cmd_bye(sock);
        break;

    default:
        break;
    }
}

void send_ftp_cmd(int sock, cmd_no cmd, int arg_len, char *arg)
{
    int i = 0, j;
    int raw_cmd_len = 1 + 4 + arg_len; //原始命令的长度(不包含帖头、帧发，没有转义之前)
    unsigned char *raw_cmd = (unsigned char *)malloc(raw_cmd_len);

    raw_cmd[0] = cmd;

    raw_cmd[1] = arg_len & 0xff;
    raw_cmd[2] = (arg_len >> 8) & 0xff;
    raw_cmd[3] = (arg_len >> 16) & 0xff;
    raw_cmd[4] = (arg_len >> 24) & 0xff;

    for (i = 5, j = 0; j < arg_len; i++, j++)
    {
        raw_cmd[i] = arg[j];
    }

    int ret = Packet_transmission(sock, raw_cmd, raw_cmd_len);

    free(raw_cmd);
}

int recv_cmd_result(int sock)
{
    unsigned char data[20] = "";
    int ret = Packet_reception(sock, data, 20);

    if (data[1] == 0x00)
    {
        printf("recv ok!\n");
        return 0;
    }
    else
    {
        printf("recv failed!\n");
        return -1;
    }
}

void recv_cmd_data(int sock)
{
    unsigned char data[2048] = "";
    int ret = Packet_reception(sock, data, 2048);
    //printf("len = %d\n",len);

    printf("%s\n", data + 5);
}

void handle_cmd_ls(int sock)
{
    send_ftp_cmd(sock, CMD_LS, 0, NULL);

    int ret = recv_cmd_result(sock);
    if (ret == 0)
    {
        //printf("recv_cmd_data\n");
        recv_cmd_data(sock);
    }
}

void handle_cmd_get(int sock, char *filename)
{
    //printf("用户想要从服务器上下载[%s]这个文件\n", filename);
    //发送命令包给服务器
    send_ftp_cmd(sock, CMD_GET, strlen(filename), filename);
    int ret = recv_cmd_result(sock);
    if (ret != 0)
    {
        return;
    }
    //2.如果服务器返回成功就接收文件大小
    unsigned char data[10] = "";
    ret = Packet_reception(sock, data, 10);

    int file_len = 0;
    for (int i = 0; i < 4; i++)
    {
        file_len += data[i] << (i * 8);
    }

    printf("文件大小为 %d \n", file_len);

    //3.接收文件
    recv_ftp_file(sock, filename, file_len);
}

int recv_ftp_file(int sock, char *filename, int file_len)
{
    char path[100] = "";
    sprintf(path, "%s/%s", FTP_PATH, filename);
    int fd = open(path, O_CREAT | O_WRONLY, 0777);
    if (fd == -1)
    {
        perror("open error");
        return -1;
    }
    int n = 0;
    unsigned char *data = (unsigned char *)malloc(4096);
    while (n < file_len)
    {
        memset(data, 0, 4096);
        int len = Packet_reception(sock, data, 4096);

        //printf("len = %d\n", len);
        int ret = write(fd, data, len);
        n += ret;
        printf("文件大小: %d,已接收: %d\n", file_len, n);
    }
    printf("接收 %s 完毕!\n",filename);
    free(data);
    close(fd);
    return 0;
}

void handle_cmd_put(int sock, char *filename)
{
    char path[100] = "";
    sprintf(path, "%s/%s", FTP_PATH, filename);
    struct stat s;
    int ret = stat(path, &s);
    if (ret == -1)
    {
        perror("");
        return;
    }
    int file_len = s.st_size;
    send_ftp_cmd(sock, CMD_PUT, strlen(filename), filename);

    ret = recv_cmd_result(sock);
    if (ret != 0)
    {
        return;
    }

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("open error");
        return;
    }
    send_file_len(sock, file_len);

    send_ftp_file(sock, fd, file_len);
}

int send_ftp_file(int sock, int send_fd, int file_len)
{
    unsigned char *buf = (unsigned char *)malloc(64);
    if (buf == NULL)
    {
        perror("malloc error");
        return -1;
    }
    printf("文件大小%d\n", file_len);
    int len = 0;
    int n;
    lseek(send_fd, SEEK_SET, 0);
    while (len < file_len)
    {
        memset(buf, 0, 64);
        n = read(send_fd, buf, 64 - 1);
        n = Packet_transmission(sock, buf, n);
        len += n;
        printf("文件大小: %d , 已发送 %d\n",file_len, len);
    }
    printf("发送完毕!\n");
    free(buf);
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

void handle_cmd_bye(int sock)
{
    send_ftp_cmd(sock,CMD_BYE,0,NULL);
    int ret = recv_cmd_result(sock);
    if(ret == 0)
    {
        printf("已退出!\n");
        exit(0);
    }
}

void ftp_client_main(const char *ip, short port)
{
    signal(SIGINT, sig_handler); //注册信号处理程序

    sock = connect_tcp_server(ip, port);

    while (!terminate)
    {
        unsigned char cmd[256] = "";
        printf("ftp_cmd $ ");
        fflush(stdout);

        fgets(cmd, 256, stdin);
        if (strlen(cmd) > 0 && cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) - 1] = '\0';

        // printf("*****%s****\n", cmd);

        if (strcmp(cmd, "ls") == 0)
        {
            // printf("用户输入了 ls 这个命令\n");
            handle_cmd_ls(sock);
        }
        else if (strncmp(cmd, "get", 3) == 0)
        {
            // printf("用户输入了 get 这个命令\n");
            char *p = cmd + 3;
            while (*p == ' ')
                p++;
            handle_cmd_get(sock, p);
        }
        else if (strncmp(cmd, "put", 3) == 0)
        {
            printf("用户输入了 put 这个命令\n");
            char *p = cmd + 3;
            while (*p == ' ')
                p++;
            handle_cmd_put(sock, p);
        }
        else if (strcmp(cmd, "bye") == 0)
        {
            printf("用户输入了 bye 这个命令\n");
            handle_cmd_bye(sock);
        }
    }
    close(sock);
}