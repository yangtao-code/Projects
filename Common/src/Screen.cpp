#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include <mutex>
#include "Screen.h"

screen::Screen *screen::Screen::instance = nullptr;

screen::Screen *screen::Screen::getInstance()
{
    static std::mutex mex;
    mex.lock();
    if (instance == nullptr)
    {
        instance = new Screen;
    }
    mex.unlock();
    return instance;
}

uint32_t screen::Screen::get_lcd_width() const
{
    return this->lcd_width;
}

uint32_t screen::Screen::get_lcd_height() const
{
    return this->lcd_height;
}

uint32_t screen::Screen::get_bits_per_pixel() const
{
    return this->bits_per_pixel;
}

screen::Screen::Screen(const char *file)
    : dev_node(file), fd(-1), plcd(nullptr),
      lcd_width(0), lcd_height(0), bits_per_pixel(0)
{
    // 1.打开屏幕文件
    this->fd = open(dev_node, O_RDWR);
    if (-1 == this->fd)
    {
        perror("open error");
        throw -1;
    }

    // 获取屏幕参数
    struct fb_var_screeninfo var;

    int ret = ioctl(fd, FBIOGET_VSCREENINFO, &var);
    if (ret == -1)
    {
        close(fd);
        perror("ioctl error");
        throw -1;
    }

    this->lcd_width = var.xres;
    this->lcd_height = var.yres;
    this->bits_per_pixel = var.bits_per_pixel;

    // std::cout << "lcd_height = " << lcd_height << std::endl;
    // std::cout << "lcd_width = " << lcd_width << std::endl;
    // std::cout << "bits_per_pixel = " << bits_per_pixel << std::endl;

    // 映射文件地址
    this->plcd = (int32_t *)mmap(nullptr, lcd_width * lcd_height * (bits_per_pixel / 8),
                                 PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (this->plcd == MAP_FAILED)
    {
        close(fd);
        perror("mmap");
        throw -1;
    }
}

screen::Screen::~Screen()
{
    if (instance != nullptr)
    {
        // 解映射
        int ret = munmap(this->plcd, lcd_height * lcd_width * bits_per_pixel / 8);
        if (ret == -1)
        {
            perror("munmap error");
        }

        close(this->fd);
        instance = nullptr;
    }
}

void screen::Screen::draw_Point(const int &x, const int &y, const uint32_t &color) const
{
    if (plcd && x >= 0 && x < lcd_width && y >= 0 && y < lcd_height)
    {
        *(plcd + lcd_width * y + x) = color;
    }
}
