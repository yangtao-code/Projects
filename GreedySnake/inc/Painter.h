#ifndef __PAINTER_H__
#define __PAINTER_H__

#include "Screen.h"
#include "Rectangle.h"
#include "Image.h"

namespace painter
{
    class Painter
    {
    private:
        const screen::Screen &s; // 屏幕
        uint32_t color;          // 画笔颜色

        /****   画布   ****/
        int width;  // 画布宽
        int height; // 画布高
        int pos_x;  // 画布在屏幕上的左上角x轴坐标
        int pos_y;  // 画布在屏幕上的左上角y轴坐标
        /*****************/

    public:
        /***
         * @brief 有参构造函数
         *
         * @param _s 屏幕
         * @param _width 窗口宽,默认为每行800像素
         * @param _height 窗口高,默认为480行
         */
        explicit Painter(const screen::Screen &_s,
                         const int &_width = 800, const int &_height = 480);

        /***
         * @brief 在画布上画一个矩形,坐标为(x0,y0)
         *
         * @param rect  矩形
         * @param x0
         * @param y0
         */
        void draw_Rectangle(const rectangle::Rectangle &rect,
                            const int &x0, const int &y0) const;
        /***
         * @brief 在屏幕s上绘制一个图片
         *
         * @param pic   图片
         * @param x0    x轴
         * @param y0    y轴
         */
        void draw_Image(const image::Image &pic, int x0, int y0) const;

        /****   以下函数用来获取或设置画家类的属性   ****/
        // 设置画笔的颜色
        void setcolor(uint32_t _color);
        // 设置画布的坐标
        void set_pos(const int &_x, const int &_y);
        // 设置画布的x轴坐标
        int get_pos_x() const;
        // 获取画布的y轴坐标
        int get_pos_y() const;

        uint32_t get_color() const;

        /**********************************************/
    };

};

#endif