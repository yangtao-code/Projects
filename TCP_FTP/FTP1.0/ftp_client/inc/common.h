#ifndef __COMMON_H__
#define __COMMON_H__

typedef enum cmd_no
{
    CMD_LS = 0x01,
    CMD_GET = 0x02,
    CMD_PUT = 0x03,
    CMD_BYE = 0x04,

    CMD_UNKNOWN = 0Xff
} cmd_no;

/***
 * @brief 用于TCP服务端，创建一个“监听套接字”
 * 
 * @param ip 服务器的ip地址
 * @param port 端口号
 * @return int  成功，返回一个监听套接字
 *              失败，失败直接退出程序并打印错误信息
 */
int create_listen_sockct(const char *ip, short port);

/*
    connect_tcp_server: 创建一个TCP的套接字，并去连接服务器
    @serv_ip: tcp服务器的ip地址
    @port: tcp服务器的端口号
    返回值：
        成功，返回连接套接字
        失败 返回-1
*/
int connect_tcp_server(const char *serv_ip, short port);

/***
 * @brief 接收一个数据包并解包,注意需要手动释放返回的地址
 * 
 * @param sock_fd 套接字描述符
 * @param len   返回实际接收的字节数,失败为-1
 * @return char* 返回数据的地址,失败返回NULL
 */
int Packet_reception(int sock_fd,unsigned char* buf, int len);

/***
 * @brief 发送数据并对数据进行处理成数据包
 * 
 * @param sock_fd 套接字描述符
 * @param buf 发送数据缓冲区
 * @param len 数据长度
 * @return int 返回数据发送的字节数,失败返回-1
 */
int Packet_transmission(int sock_fd, const unsigned char *buf, int len);

void perr_exit(const char *s);

/***
 * @brief 提取一个
 * 
 * @param fd 套接字描述符
 * @param sa 
 * @param salenptr 结构体长度
 * @return int 成功返回客户端的文件描述符,失败返回-1并打印错误信息
 */
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);

/***
 * @brief 
 * 
 * @param fd 套接字
 * @param sa 
 * @param salen 
 * @return int 成功返回0,失败直接退出程序并打印错误信息
 */
int Bind(int fd, const struct sockaddr *sa, socklen_t salen);

/***
 * @brief 连接一个服务器
 * 
 * @param fd 
 * @param sa 
 * @param salen 
 * @return int 成功返回0,失败直接退出程序并打印错误信息
 */
int Connect(int fd, const struct sockaddr *sa, socklen_t salen);

/***
 * @brief 监听一个套接字
 * 
 * @param fd 
 * @param backlog 监听的最大数
 * @return int 成功返回0,失败直接退出程序并打印错误信息
 */
int Listen(int fd, int backlog);

/***
 * @brief 创建一个套接字
 * 
 * @param family 协议
 * @param type  类型
 * @param protocol 
 * @return int 成功返回一个套接字描述符,失败直接退出程序并打印错误信息
 */
int Socket(int family, int type, int protocol);

#endif