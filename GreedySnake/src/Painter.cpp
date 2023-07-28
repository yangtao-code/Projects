#include <iostream>

#include "Painter.h"

void painter::Painter::draw_Rectangle(const rectangle::Rectangle *rect,
                                      const int &x0, const int &y0)const
{
    rect->draw(this->s, this->pos_x + x0, this->pos_y + y0);
}

painter::Painter::Painter(const screen::Screen &_s,
                          const int &_width, const int &_height)
    : s(_s), width(_width), height(_height), pos_x(0), pos_y(0)
{
}
