#include "input.h"
#include <stdio.h>

Direction get_direction()
{
    Direction d;
    char c;
    scanf("%c", &c);
    getchar(); // 从输入缓冲区读取多余的\n, 丢弃不要
    printf("c = %d \n", c);
    switch (c)
    {
    case 'w':
        d = UP;
        break;
    case 's':
        d = DOWN;
        break;
    case 'a':
        d = LEFT;
        break;
    case 'd':
        d = RIGHT;
        break;
    default:
        d = UNKNOW;
        break;
    }
    return d;
}