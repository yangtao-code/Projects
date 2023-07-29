#include <iostream>
#include "Painter.h"

void painter::Painter::draw_Image(const image::Image &pic, int x0, int y0) const
{
    pic.draw(this->s, this->pos_x + x0, this->pos_y + y0);
}

void painter::Painter::draw_Rectangle(const rectangle::Rectangle &rect,
                                      const int &x0, const int &y0) const
{
    rect.draw(this->s, this->pos_x + x0, this->pos_y + y0);
}

painter::Painter::Painter(const screen::Screen &_s,
                          const int &_width, const int &_height)
    : s(_s), width(_width), height(_height), pos_x(0), pos_y(0)
{
}

void painter::Painter::set_pos(const int &_x, const int &_y)
{
    this->pos_x = _x;
    this->pos_y = _y;
}

void painter::Painter::setcolor(uint32_t _color)
{
    this->color = _color;
}

int painter::Painter::get_pos_x() const
{
    return this->pos_x;
}

int painter::Painter::get_pos_y() const
{
    return this->pos_y;
}

uint32_t painter::Painter::get_color() const
{
    return this->color;
}