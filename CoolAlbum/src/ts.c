#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include "ts.h"

int ts_fd = -1;

int open_ts()
{
    ts_fd = open("/dev/input/event0", O_RDWR);
    if (ts_fd < 0)
    {
        perror("open /dev/input/event0 failed");
        return -1;
    }
    return 0;
}

int getway()
{
    struct input_event ev;
    // 触摸第一次的坐标
    int x0 = -1, y0 = -1;
    // 实时坐标,最终为手最后一次离开触摸屏的
    int x, y;
    while (1)
    {
        int n = read(ts_fd, &ev, sizeof(struct input_event));
        if (n != sizeof(struct input_event))
        {
            continue;
        }
        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            if (x0 == -1)
            {
                x0 = ev.value;
            }
            x = ev.value;
        }
        if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (y0 == -1)
            {
                y0 = ev.value;
            }
            y = ev.value;
        }
        if (ev.type == EV_KEY && ev.code == 330 && ev.value == 0)
            break;
    }
    x0 = x0 * 800 / 1024;
    x = x * 800 / 1024;
    y0 = y0 * 480 / 600;
    y = y * 480 / 600;
    printf("触摸开始坐标(%d , %d)\n", x0, y0);
    printf("触摸结束坐标(%d , %d)\n", x, y);
    if (x0 == x && y0 == y)
    {
        if (x >= 770 && x < 800 && y >= 0 && y < 20)
        {
            printf("SHUT\n");
            return SHUT;
        }
        return -1;
    }
    // 根据斜率k获得方向值
    int k = abs((y - y0) / (double)(x - x0));
    // 上下方向
    if (k >= 1)
    {
        if (y > y0)
        {
            printf("DOWN\n");
            return DOWN;
        }
        else if (y < y0)
        {
            printf("UP\n");
            return UP;
        }
    }
    else
    {
        if (x > x0)
        {
            printf("RIGHT\n");
            return RIGHT;
        }
        else if (x < x0)
        {
            printf("LEFT\n");
            return LEFT;
        }
    }
}

void get_location(int *x, int *y)
{
    *x = -1;
    *y = -1;
    struct input_event ev;
    while (1)
    {
        int n = read(ts_fd, &ev, sizeof(struct input_event));
        if (n != sizeof(struct input_event))
        {
            continue;
        }
        // 是触摸事件且为x轴
        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            if (*x == -1)
                *x = ev.value * 800 / 1024;
        }
        // 是触摸事件且为y轴
        else if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (*y == -1)
                *y = ev.value * 480 / 600;
        }
        else if (ev.type = EV_KEY && ev.code == BTN_TOUCH)
        {
            if (ev.value == 1)
            {
                printf("press,type = %d,code = %d,value = %d\n", ev.type, ev.code, ev.value);
            }
            else if (ev.value == 0)
            {
                printf("leave,type = %d,code = %d,value = %d\n", ev.type, ev.code, ev.value);
                break;
            }
        }
    }
    printf("触摸的屏幕坐标为(%d , %d)\n", *x, *y);
}

void Close_software()
{
    printf("程序正在退出!\n");
    sleep(1);
    printf("退出成功!\n");
    exit(0);
}

int close_ts()
{
    int ret = close(ts_fd);
    if (ret < 0)
    {
        perror("close /dev/input/event0 failed");
        return -1;
    }
    return 0;
}
