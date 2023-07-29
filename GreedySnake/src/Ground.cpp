#include <iostream>

#include "Ground.h"

void Ground::draw(const screen::Screen &s, const int &x0, const int &y0) const
{
    Rectangle::draw(s, x0, y0);

    // 画点的方式
    //  for (int j = 0; j < get_h(); j += d)
    //  {
    //      for (int i = 0; i < get_w(); i += d)
    //      {
    //          s.draw_Point(i, j, line_color);
    //      }
    //  }

    // 画线的方式
    rectangle::Rectangle tmp(800, 480, this->line_color);
    for (int i = 0; i <= this->get_w(); i += this->d)
    {
        tmp.resize(1, get_h());
        tmp.draw(s, x0 + i, y0);
    }
    for (int i = 0; i <= this->get_h(); i += this->d)
    {
        tmp.resize(get_w(), 1);
        tmp.draw(s, x0, y0 + i);
    }
}

void Ground::setline_color(const uint32_t &_line_color)
{
    this->line_color = _line_color;
}

Ground::Ground(const uint32_t &_w, const uint32_t &_h, const int &_d,
               const uint32_t &_fill_color, const uint32_t &_line_color)
    : Rectangle(_w, _h, _fill_color), d(_d), line_color(_line_color)
{
}
