#include <stdio.h>
#include <stdlib.h>
#include "ftp_server.h"

//本程序的用法说明
void usage(void)
{
    printf("本程序是一个 文件传输的 服务器程序\n");
    printf("本程序运行需要带两个参数： 一个本机的Ip地址,第二个端口号\n");
    printf("如:  ftp_server 172.8.0.2  9999\n");
}

int main(int argc, char const *argv[])
{
    //判断命令是否出错
    if (argc != 3)
    {
        usage();
        exit(0);
    }
    //运行ftp服务器
    ftp_server_main(argv[1],atoi(argv[2]));
    
    return 0;
}
