#ifndef __HANDLER_H__
#define __HANDLER_H__

typedef struct client_info
{
    int cfd;
    int epfd;
    struct epoll_event ev;
} client_info_t;

#include "common.h"

/***
 * @brief 这是一个线程池的任务函数,用来处理客户端发过来的指令并识别指令
 * 
 * @param arg client_info_t 参数
 */
void handle_connection(void* arg);

/***
 * @brief 命令处理函数
 * 
 * @param cfd 客户端的套接字描述符
 * @param arg 命令参数,NULL表示没有参数
 * @param arg_int 参数长度
 */
void handle_cmd(client_info_t* c, cmd_no cmd, const unsigned char *arg, int arg_len);

/***
 * @brief 处理客户端发来的ls命令
 * 
 * @param cfd 客户端的套接字描述符
 * @param arg 命令参数
 * @param arg_int 命令参数长度
 */
void handle_cmd_ls(int cfd, const unsigned char *arg, int arg_len);

/***
 * @brief 处理客户端发来的get命令
 * 
 * @param cfd 客户端的套接字描述符
 * @param arg 命令参数
 * @param arg_len 命令参数长度
 */
void handle_cmd_get(int cfd, const unsigned char *arg, int arg_len);

/***
 * @brief 发送文件长度给客户端
 * 
 * @param cfd 客户端的文件描述符
 * @param file_len 文件大小
 */
void send_file_len(int cfd, int file_len);

/***
 * @brief 发送文件给客户端
 * 
 * @param cfd 客户端的文件描述符
 * @param send_fd 要发送的文件描述符
 * @param file_len 文件大小
 */
void send_file(int cfd, int send_fd, int file_len);

/***
 * @brief 处理客户端发来的put命令
 * 
 * @param cfd 客服端的文件描述符
 * @param arg 命令参数
 * @param arg_len 命令参数长度
 */
void handle_cmd_put(int cfd, const unsigned char *arg, int arg_len);

/***
 * @brief 接收客户端发过来的文件
 * 
 * @param cfd 客户端的文件描述符
 * @param filename 文件名
 * @param file_len 文件大小
 * @return int 
 */
int recv_ftp_file(int cfd, const char *filename, int file_len);

/***
 * @brief 处理客户端发过来的bye命令
 * 
 * @param cfd 客户端的文件描述符
 * @param arg 命令参数
 * @param arg_len 参数长度
 */
void handle_cmd_bye(int cfd, const unsigned char *arg, int arg_len);

/***
 * @brief 发送命令执行结果给客户端
 * 
 * @param cfd 客户端的套接字描述符
 * @param cmd 命令号
 * @param result 结果号,0x00表示成功发送
 */
void send_execution_result(int cfd, cmd_no cmd, unsigned char result);

/***
 * @brief 发生命令的处理结果给客户端
 * 
 * @param cfd 客户端的文件描述符
 * @param cmd 命令号
 * @param data 结果数据
 * @param data_len 数据长度
 */
void send_cmd_data(int cfd, cmd_no cmd, char *data, int data_len);

#endif