#include <stdio.h>
#include <stdlib.h>

#include "ftp_client.h"

//本程序的用法说明
void usage(void)
{
    printf("本程序是一个 文件传输的 客户端程序\n");
    printf("本程序运行需要带两个参数： 一个服务器的Ip地址,第二个为服务器端口号\n");
    printf("如:  ftp_client 172.8.0.2  9999\n");
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        usage();
        exit(0);
    }

    ftp_client_main(argv[1],atoi(argv[2]));

    return 0;
}
