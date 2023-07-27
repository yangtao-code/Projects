#include <iostream>
#include <unistd.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "TouchEvent.h"

#define NUM 600000

void tevent::TouchEvent::event(const touch::Touch &ts)
{
    struct input_event ev;
    int num; // 触摸的按压值
    // 初始化事件信息
    init_event();

    while (1)
    {
        // 1.刚触摸触摸屏时
        if (ev.type == EV_KEY && ev.code == 330 && ev.value == 1)
        {
            num = 0;
        }
        num++;
        // 读触摸屏文件
        int n = read(ts.get_fd(), &ev, sizeof(struct input_event));

        if (n != sizeof(struct input_event))
        {
            continue;
        }
        // 2.触摸过程
        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            // 如果this->first_pos.x == -1说明刚触摸
            if (this->first_pos.x == -1)
            {
                this->first_pos.x = ev.value; // 触摸x轴起点
            }
            this->last_pos.x = ev.value; // 不断修改最后触摸的x轴位置
        }
        if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (this->first_pos.y == -1)
            {
                this->first_pos.y = ev.value; // 触摸y轴起点
            }
            this->last_pos.y = ev.value; // 不断修改最后触摸的y轴位置
        }
        // 3.触摸离开时
        if (ev.type == EV_KEY && ev.code == 330 && ev.value == 0)
        {
            std::cout << "leave..." << std::endl;
            break;
        }
    }

    handle(num);
}

void tevent::TouchEvent::handle(const int &num)
{
    // 屏幕坐标处理,因为lcd屏幕是800*480 而触摸屏是1024 * 600的
    this->first_pos.x = this->first_pos.x * 800 / 1024;
    this->last_pos.x = this->last_pos.x * 800 / 1024;
    this->first_pos.y = this->first_pos.y * 480 / 600;
    this->last_pos.y = this->last_pos.y * 480 / 600;

    // 根据斜率k获得方向值
    int k = (last_pos.y - first_pos.y) / (double)(last_pos.x - first_pos.x);

    printf("触摸开始坐标(%d , %d)\n", first_pos.x, first_pos.y);
    printf("触摸结束坐标(%d , %d)\n", last_pos.x, last_pos.y);
    printf("num = %d , k = %d\n", num, k);

    // 如果满足下面表达式就认为是点击或者按压事件
    if (Abs(this->last_pos.x - this->first_pos.x) <= 3 &&
        Abs(this->last_pos.y - this->first_pos.y) <= 2)
    {
        // 如果num >= NUM 就认为是按压事件
        if (num > NUM)
        {
            this->s = PRESS;
            std::cout << "PRESS" << std::endl;
        }
        // 否则为点击事件
        else
        {
            this->s = CLICK;
            std::cout << "CLICK" << std::endl;
        }
        return;
    }

    // 否则为其他事件
    // 上下方向
    if (Abs(k) >= 1)
    {
        if (this->last_pos.y > this->first_pos.y)
        {
            this->s = DOWN;
            std::cout << "DOWN" << std::endl;
            return;
        }
        else
        {
            this->s = UP;
            std::cout << "UP" << std::endl;
            return;
        }
    }

    if (this->last_pos.x > this->first_pos.x)
    {
        this->s = RIGHT;
        std::cout << "RIGHT" << std::endl;
    }
    else
    {
        this->s = LEFT;
        std::cout << "LEFT" << std::endl;
    }
}

uint32_t tevent::TouchEvent::Abs(const int &num)
{
    return num >= 0 ? num : 0 - num;
}

void tevent::TouchEvent::init_event()
{
    this->first_pos.x = -1;
    this->first_pos.y = -1;

    this->last_pos.x = 0;
    this->last_pos.y = 0;

    this->s = NUL;
}

tevent::TouchEvent::TouchEvent()
{
    this->init_event();
}
