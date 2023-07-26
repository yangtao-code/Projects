/*  触摸屏有关函数  */
#ifndef __TS_H__
#define __TS_H__

typedef enum WAY
{
    SHUT = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
} WAY;

/**
 * @brief 初始化触摸屏函数,打开触摸屏
 *
 * @return int 成功返回0,失败返回-1
 */
int open_ts();

/**
 * @brief 关闭触摸屏
 *
 */
int close_ts();

/**
 * @brief 获得触摸屏的触摸方向
 *
 * @return int 返回方向值
 */
int getway();
/**
 * @brief 获得触摸的坐标
 *
 */
void get_location(int *x, int *y);

/**
 * @brief 程序退出线程函数
 *
 * @param arg
 * @return void*
 */
void Close_software();

#endif
