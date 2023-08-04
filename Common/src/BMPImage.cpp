#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "BMPImage.h"

void BMPImage::draw(const screen::Screen &s, int x0, int y0) const
{

    int fd = open(image_path.c_str(), O_RDONLY);
    if (fd == -1)
    {
        std::cout << "open " << image_path << " error";
        perror("");
        throw -1;
    }

    // 获取像素数组
    // 一行的有效字节数
    int line_valid_bytes = abs(width) * bytes_per_pixel;
    int line_bytes; // 一行的实际字节数 = 一行的有效字节数 + 赖子
    int laizi = 0;  // 赖子
    if (line_valid_bytes % 4 != 0)
    {
        laizi = 4 - line_valid_bytes % 4;
    }
    line_bytes = line_valid_bytes + laizi;
    int bytes = line_bytes * abs(height); // 像素数组大小
    uint8_t bmp_buf[bytes];               // 定义色素数组缓冲区
    lseek(fd, 0x36, SEEK_SET);

    int n = read(fd, bmp_buf, bytes); // 读bmp的色素数组
    if (n != bytes)
    {
        std::cout << "read bmp error" << std::endl;
        close(fd);
        throw -1;
    }
    // 开始绘制图片
    int i = 0;
    char a, r, g, b;    // 透明度和三原色
    unsigned int color; // 颜色
    for (int y = 0; y < abs(height); y++)
    {
        for (int x = 0; x < abs(width); x++)
        {
            b = bmp_buf[i++];
            g = bmp_buf[i++];
            r = bmp_buf[i++];
            if (bytes_per_pixel == 4)
            {
                a = bmp_buf[i++];
            }
            else if (bytes_per_pixel == 3)
            {
                a = 0;
            }
            color = a << 24 | r << 16 | g << 8 | b;
            s.draw_Point(width > 0 ? x + x0 : abs(width) - x - 1 + x0,
                         height > 0 ? abs(height) - y - 1 + y0 : y + y0,
                         color);
        }
        i = i + laizi;
    }
    // 关闭bmp图片
    close(fd);
}

bool BMPImage::init_Image()
{
    // 1.打开bmp图片
    int fd = open(image_path.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("open bmp error");
        return false;
    }
    lseek(fd, 0, SEEK_SET);

    uint8_t buf[2] = "";
    // 图片的宽、高、色深
    int _width = 0, _height = 0;
    uint16_t _bits_per_pixel = 0;
    // 识别该图片是否为bmp图片
    int n = read(fd, (void *)buf, 2);
    if (buf[0] != 0x42 || buf[1] != 0x4d || n != 2)
    {
        std::cout << image_path << "not bmp or The bmp is damged." << std::endl;
        close(fd);
        return false;
    }

    // 读bmp头,宽,高,深度
    lseek(fd, 0x12, SEEK_SET);
    read(fd, &_width, 4);

    lseek(fd, 0x16, SEEK_SET);
    read(fd, &_height, 4);

    lseek(fd, 0x1c, SEEK_SET);
    read(fd, &_bits_per_pixel, 2);

    // std::cout << image_path << ": "
    //           << _width << " " << _height << " "
    //           << _bits_per_pixel << std::endl;

    this->width = _width;
    this->height = _height;
    this->bits_per_pixel = _bits_per_pixel;
    this->bytes_per_pixel = _bits_per_pixel / 8;
    this->image_size = abs(_width) * abs(_height) * _bits_per_pixel / 8;
    close(fd);
    return true;
}

BMPImage::BMPImage(const std::string &_image_path)
    : Image(_image_path)
{
    bool ret = this->init_Image();
    if (ret == false)
        throw -1;
}

bool BMPImage::updateImage(const std::string &_image_path)
{
    this->set_image_path(_image_path);
    bool ret = this->init_Image();
    if (ret == false)
        throw -1;
}