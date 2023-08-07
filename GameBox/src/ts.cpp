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

Ts::Ts()
{
    ts_fd = open("/dev/input/event0", O_RDWR | O_NONBLOCK);
    if (ts_fd < 0)
    {
        perror("open /dev/input/event0 failed");
        exit(-1);
    }
}

int Ts::get_state()
{
    
    lseek(ts_fd, 0, SEEK_SET);
    struct input_event ev;
    // 触摸第一次的坐标
    s.x0 = -1;
    s.y0 = -1;
    int num = 0;
    // 实时坐标,最终为手最后一次离开触摸屏的
    while (1)
    {
        if (ev.type == EV_KEY && ev.code == 330 && ev.value == 1)
        {
            num = 0;
        }
        num++;
        int n = read(ts_fd, &ev, sizeof(struct input_event));
        if (n != sizeof(struct input_event))
        {
            continue;
        }
        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            if (s.x0 == -1)
            {
                s.x0 = ev.value;
            }
            s.x = ev.value;
        }
        if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (s.y0 == -1)
            {
                s.y0 = ev.value;
            }
            s.y = ev.value;
        }
        if (ev.type == EV_KEY && ev.code == 330 && ev.value == 0)
        {
            printf("leave\n");
            break;
        }
    }

    s.is_used = false;

    s.x0 = s.x0 * 800 / 1024;
    s.x = s.x * 800 / 1024;
    s.y0 = s.y0 * 480 / 600;
    s.y = s.y * 480 / 600;
    printf("触摸开始坐标(%d , %d)\n", s.x0, s.y0);
    printf("触摸结束坐标(%d , %d)\n", s.x, s.y);
    printf("num = %d\n", num);
    if (s.x0 == s.x && s.y0 == s.y)
    {

        if (num > 400000)
        {
            printf("PRESS\n");
            s.way = PRESS;
        }
        else
        {
            printf("CLICK\n");
            s.way = CLICK;
        }
    }
    else
    {
        // 根据斜率k获得方向值
        int k = abs((s.y - s.y0) / (double)(s.x - s.x0));
        // 上下方向
        if (k >= 1)
        {
            if (s.y > s.y0)
            {
                printf("DOWN\n");
                s.way = DOWN;
            }
            else if (s.y < s.y0)
            {
                printf("UP\n");
                s.way = UP;
            }
        }
        else
        {
            if (s.x > s.x0)
            {
                printf("RIGHT\n");
                s.way = RIGHT;
            }
            else if (s.x < s.x0)
            {
                printf("LEFT\n");
                s.way = LEFT;
            }
        }
    }
}

Ts::~Ts()
{
    int ret = close(ts_fd);
    if (ret < 0)
    {
        perror("close /dev/input/event0 failed");
        exit(-1);
    }
}
