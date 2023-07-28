#ifndef __GROUND_H__
#define __GROUND_H__

#include <iostream>
#include "Rectangle.h"

class Ground
    : public rectangle::Rectangle
{
private:
    uint32_t line_color; // 线的颜色
    int d;               // 每条线的距离
public:
    /***
     * @brief Construct a new Ground object
     *
     * @param _w 宽
     * @param _h 高
     * @param _d 线条之间的距离
     * @param _fill_color 填充颜色
     * @param _line_color 线条颜色
     */
    Ground(const uint32_t &_w, const uint32_t &_h, const int &_d = 20,
           const uint32_t &_fill_color = 0xffffff, const uint32_t &_line_color = 0x00);

    /***
     * @brief 在屏幕s上画一个草坪
     *
     * @param s 屏幕
     * @param x0 x轴
     * @param y0 y轴
     */
    virtual void draw(const screen::Screen &s, const int &x0, const int &y0) const;


    /****   以下函数用来设置或获取草坪的属性   ****/
    /***
     * @brief 设置草坪线的颜色
     *
     * @param _line_color 颜色值
     */
    void setline_color(const uint32_t &_line_color);

    /***
     * @brief 设置草坪线之间的距离
     *
     * @param _d 距离
     */
    void set_d(const int &_d)
    {
        this->d = _d;
    }

    // 获取草坪线的距离
    int get_d() const
    {
        return d;
    }

    // 获取草坪线的颜色
    uint32_t get_line_color() const
    {
        return this->line_color;
    }

    /***********************************/

};

#endif