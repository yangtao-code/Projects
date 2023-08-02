#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "common.h"

void handle_connection(int cfd);

/***
 * @brief 命令处理函数
 * 
 * @param cfd 客户端的套接字描述符
 * @param arg 命令参数,NULL表示没有参数
 * @param arg_int 参数长度
 */
void handle_cmd(int cfd,cmd_no cmd,const unsigned char* arg,int arg_len);

/***
 * @brief 处理客户端发来的ls命令
 * 
 * @param cfd 客户端的套接字描述符
 * @param arg 
 * @param arg_int 
 */
void handle_cmd_ls(int cfd,const unsigned char* arg,int arg_len);

void handle_cmd_get(int cfd,const unsigned char* arg,int arg_len);

void send_file_len(int cfd,int file_len);

void send_file(int cfd,int send_fd,int file_len);

void handle_cmd_put(int cfd,const unsigned char* arg,int arg_len);

int recv_ftp_file(int cfd,const char *filename,int file_len);

void handle_cmd_bye(int cfd,const unsigned char* arg,int arg_len);

/***
 * @brief 发送命令执行结果给客户端
 * 
 * @param cfd 客户端的套接字描述符
 * @param cmd 命令号
 * @param result 结果号,0x00表示成功发送
 */
void send_execution_result(int cfd,cmd_no cmd,unsigned char result);

void send_cmd_data(int cfd,cmd_no cmd,char *data,int data_len);


#endif 