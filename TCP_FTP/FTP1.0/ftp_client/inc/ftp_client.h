#ifndef __FTP_CLIENT_H__
#define __FTP_CLIENT_H__


#include <arpa/inet.h>
#include <sys/socket.h>

#include "common.h"

/***
 * @brief FTP客户端运行的主程序
 * 
 * @param ip IP地址
 * @param port 端口
 */
void ftp_client_main(const char* ip,short port);

/*
    send_ftp_cmd: 构造一个ftp命令，并发送到套接字
    @sock: 套接字描述符
    @cmd: 命令号
    @arg_len: 命令参数长度
    @arg: 命令参数
    返回值:
        无。
*/
void send_ftp_cmd(int sock, cmd_no cmd, int arg_len, char *arg);

/***
 * @brief 接收发送给fpt的命令执行结果
 * 
 * @param sock 套接字
 */
int recv_cmd_result(int sock);

/***
 * @brief 接收发送给ftp的命令的返回内容
 * 
 * @param sock 
 */
void recv_cmd_data(int sock);

/***
 * @brief 处理ls命令
 * 
 * @param sock 
 */
void handle_cmd_ls(int sock);

void handle_cmd_get(int sock, char *filename);

int recv_ftp_file(int sock,char *filename,int file_len);

int send_ftp_file(int sock,int send_fd, int file_len);

void send_file_len(int cfd,int file_len);

void handle_cmd_put(int sock, char *filename);

void handle_cmd_bye(int sock);



#endif