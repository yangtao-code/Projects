#ifndef __COOLALBUM_H__
#define __COOLALBUM_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include "lcd.h"
#include "ts.h"
#include "linklist.h"

/**
 * @brief 电子相册图片显示
 *
 * @param list
 */
void CoolAlbum(Linklist *list);

/**
 * @brief 创建bmp图片显示链表
 *
 * @param dirpath 目录名
 * @return Linklist* 返回一个链表,里面存储了显示的bmp图片路径
 */
Linklist *create_piclist(const char *dirpath);

#endif