#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Screen.h"

namespace image
{
    // 图片类
    class Image
    {
    protected:
        std::string image_path;   // 图片路径
        int width;           // 图片的宽
        int height;          // 图片的高
        uint32_t bits_per_pixel;  // 图片每个像素点的色深
        uint32_t bytes_per_pixel; // 图片每个像素点占多少字节
        uint32_t image_size;      // 图片的大小

    protected:
        /***
         * @brief 初始化图片信息
         *
         * @return true 成功返回true
         * @return false 失败返回false
         */
        virtual bool init_Image() = 0;

    public:
        // 构造函数
        Image(const std::string &_image_path);

        /***
         * @brief 在屏幕s上显示图片,坐标为(x0,y0)
         *
         * @param s 屏幕
         * @param x0    x轴坐标
         * @param y0    y轴坐标
         */
        virtual void draw(const screen::Screen &s, int x0, int y0) const = 0;

        /****   以下函数用来获取或设置图片   ****/

        /***
         * @brief 更好图片并更新图片信息
         *
         * @param _image_path 图片路径
         * @return true 成功返回true
         * @return false 失败返回false
         */
        virtual bool updateImage(const std::string &_image_path) = 0;
        // 设置图片路径
        void set_image_path(const std::string &_image_path);

        // void set_width(const uint32_t &_width);
        // void set_height(const uint32_t &_height);
        // void set_bits_per_pixel(const uint32_t &_bits_per_pixel);

        // 获取图片的宽
        int get_width() const;
        // 获取图片的高
        int get_height() const;
        // 获取图片的色深
        uint32_t get_bits_per_pixel() const;
        // 获取图片路径
        std::string get_image_path() const;
        // 获取图片大小
        uint32_t get_image_size() const;
        /**************************************/
    };

};

#endif