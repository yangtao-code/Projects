#include <iostream>
#include "Rectangle.h"

void rectangle::Rectangle::draw(const screen::Screen &s, const int &x0, const int &y0) const
{
    for (int y = y0; y < y0 + h; y++)
    {
        for (int x = x0; x < x0 + w; x++)
        {
            s.draw_Point(x, y, fill_color);
        }
    }
}

void rectangle::Rectangle::resize(const uint32_t &_w, const uint32_t &_h)
{
    this->w = _w;
    this->h = _h;
}

rectangle::Rectangle::Rectangle(const uint32_t &_w, const uint32_t &_h,
                                const uint32_t &_color)
    : w(_w), h(_h), fill_color(_color)
{
    //std::cout << "Rectangle()有参构造函数的调用" << std::endl;
}

void rectangle::Rectangle::Rectangle::setfill_color(const uint32_t &_color)
{
    this->fill_color = _color;
}