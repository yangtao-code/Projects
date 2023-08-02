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
void ftp_client_main(const char *ip, short port);

/***
 * @brief 构造一个ftp命令，并发送到套接字
 * 
 * @param sock 套接字描述符
 * @param cmd 命令号
 * @param arg_len 命令参数长度
 * @param arg 命令参数
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
 * @param sock 服务器的文件描述符
 */
void handle_cmd_ls(int sock);

/***
 * @brief 处理get命令
 * 
 * @param sock 服务器的文件描述符
 * @param filename 文件名
 */
void handle_cmd_get(int sock, char *filename);

/***
 * @brief 接收服务器发过来的文件
 * 
 * @param sock 套接字描述符
 * @param filename 文件名
 * @param file_len 文件大小
 * @return int 返回实际接收的文件字节数
 */
int recv_ftp_file(int sock, char *filename, int file_len);

/***
 * @brief 发送文件给服务器
 * 
 * @param sock 套接字描述符
 * @param send_fd 要发送的文件描述符
 * @param file_len 文件读写
 * @return int 返回实际发送的文件字节数
 */
int send_ftp_file(int sock, int send_fd, int file_len);

/***
 * @brief 发生文件大小给服务器
 * 
 * @param sock  套接字描述符
 * @param file_len 文件大小
 */
void send_file_len(int sock, int file_len);

/***
 * @brief 处理put命令
 * 
 * @param sock 套接字描述符
 * @param filename 文件名
 */
void handle_cmd_put(int sock, char *filename);

/***
 * @brief 处理bye命令
 * 
 * @param sock 套接字描述符
 */
void handle_cmd_bye(int sock);

#endif